//
//  sound.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 03/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include "sound.hpp"
#include <map>
#include <memory>
#include <ddui/util/get_asset_filename>

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#include <soundio/soundio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

namespace sound {

struct StereoBuffer {
    int sample_rate;
    int number_of_samples;
    std::unique_ptr<float[]> samples;
};

static std::map<std::string, StereoBuffer> sounds;

static struct SoundIo* soundio;
static struct SoundIoDevice* device;
static struct SoundIoOutStream* outstream;

static StereoBuffer* buffer_to_play = NULL;
static int samples_played = 0;

static StereoBuffer load_ogg(const char* filename) {
    auto asset_filename = get_asset_filename(filename);

    int channels, sample_rate;
    short* samples;
    
    auto number_of_samples = stb_vorbis_decode_filename(asset_filename.c_str(), &channels, &sample_rate, &samples);
    if (number_of_samples == -1) {
        StereoBuffer buffer;
        buffer.sample_rate = -1;
        return buffer;
    }

    StereoBuffer buffer;
    buffer.sample_rate = sample_rate;
    buffer.number_of_samples = number_of_samples;
    buffer.samples = std::unique_ptr<float[]>(new float[number_of_samples * 2]);

    auto ptr_in = &samples[0];
    auto ptr_end = &samples[number_of_samples * channels];
    auto ptr_out = buffer.samples.get();

    if (channels == 1) {
        
        while (ptr_in < ptr_end) {
            *ptr_out++ = (float)(*ptr_in) / 32768.0;
            *ptr_out++ = (float)(*ptr_in++) / 32768.0;
        }

    } else if (channels == 2) {
    
        while (ptr_in < ptr_end) {
            *ptr_out++ = (float)(*ptr_in++) / 32768.0;
        }

    } else {
        printf("Sound file is neither stereo nor mono....\n");
        StereoBuffer buffer;
        buffer.sample_rate = -1;
        return buffer;
    }

    return buffer;
}

static void write_sample_s16ne(char *ptr, float sample) {
    int16_t *buf = (int16_t *)ptr;
    double range = (float)INT16_MAX - (float)INT16_MIN;
    double val = sample * range / 2.0;
    *buf = val;
}

static void write_sample_s32ne(char *ptr, float sample) {
    int32_t *buf = (int32_t *)ptr;
    double range = (float)INT32_MAX - (float)INT32_MIN;
    double val = sample * range / 2.0;
    *buf = val;
}

static void write_sample_float32ne(char *ptr, float sample) {
    float *buf = (float *)ptr;
    *buf = sample;
}

static void write_sample_float64ne(char *ptr, float sample) {
    double *buf = (double *)ptr;
    *buf = (double)sample;
}

static void (*write_sample)(char *ptr, float sample);
static void write_callback(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) {
    bool should_pause = false;

    struct SoundIoChannelArea *areas;
    int err;

    int frames_left = frame_count_max;

    for (;;) {
        int frame_count = frames_left;
        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;

        const struct SoundIoChannelLayout *layout = &outstream->layout;
        if (layout->channel_count < 2) {
            break;
        }
        
        if (buffer_to_play == NULL) {
            for (int frame = 0; frame < frame_count; frame += 1) {
                write_sample(areas[0].ptr, 0.0);
                write_sample(areas[1].ptr, 0.0);
                areas[0].ptr += areas[0].step;
                areas[1].ptr += areas[1].step;
            }
            should_pause = true;
        } else {
            int samples_left = buffer_to_play->number_of_samples - samples_played;
            int num_frames = frame_count < samples_left ? frame_count : samples_left;
            
            auto buffer_ptr = buffer_to_play->samples.get() + (samples_played * 2);
            for (int frame = 0; frame < num_frames; frame += 1) {
                write_sample(areas[0].ptr, *buffer_ptr++);
                write_sample(areas[1].ptr, *buffer_ptr++);
                areas[0].ptr += areas[0].step;
                areas[1].ptr += areas[1].step;
            }
            for (int frame = num_frames; frame < frame_count; frame += 1) {
                write_sample(areas[0].ptr, 0.0);
                write_sample(areas[1].ptr, 0.0);
                areas[0].ptr += areas[0].step;
                areas[1].ptr += areas[1].step;
            }
            
            samples_played += num_frames;
            if (samples_left <= num_frames) {
                buffer_to_play = NULL;
                samples_played = 0;
                should_pause = true;
            }
        }

        if ((err = soundio_outstream_end_write(outstream))) {
            if (err == SoundIoErrorUnderflow)
                return;
            fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }

    #ifdef __APPLE__
    soundio_outstream_pause(outstream, should_pause);
    #endif
}

static void underflow_callback(struct SoundIoOutStream *outstream) {
    static int count = 0;
    fprintf(stderr, "underflow %d\n", count++);
}

int init_audio_client() {
    char *stream_name = NULL;
    double latency = 0.0;
    int sample_rate = 0;

    soundio = soundio_create();
    if (!soundio) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    int err = soundio_connect(soundio);

    if (err) {
        fprintf(stderr, "Unable to connect to backend: %s\n", soundio_strerror(err));
        return 1;
    }

    fprintf(stderr, "Backend: %s\n", soundio_backend_name(soundio->current_backend));

    soundio_flush_events(soundio);

    int device_index = soundio_default_output_device_index(soundio);
    if (device_index < 0) {
        fprintf(stderr, "Output device not found\n");
        return 1;
    }

    device = soundio_get_output_device(soundio, device_index);
    if (!device) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    fprintf(stderr, "Output device: %s\n", device->name);

    if (device->probe_error) {
        fprintf(stderr, "Cannot probe device: %s\n", soundio_strerror(device->probe_error));
        return 1;
    }

    outstream = soundio_outstream_create(device);
    if (!outstream) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    outstream->write_callback = write_callback;
    outstream->underflow_callback = underflow_callback;
    outstream->name = stream_name;
    outstream->software_latency = latency;
    outstream->sample_rate = sample_rate;

    if (soundio_device_supports_format(device, SoundIoFormatFloat32NE)) {
        outstream->format = SoundIoFormatFloat32NE;
        write_sample = write_sample_float32ne;
    } else if (soundio_device_supports_format(device, SoundIoFormatFloat64NE)) {
        outstream->format = SoundIoFormatFloat64NE;
        write_sample = write_sample_float64ne;
    } else if (soundio_device_supports_format(device, SoundIoFormatS32NE)) {
        outstream->format = SoundIoFormatS32NE;
        write_sample = write_sample_s32ne;
    } else if (soundio_device_supports_format(device, SoundIoFormatS16NE)) {
        outstream->format = SoundIoFormatS16NE;
        write_sample = write_sample_s16ne;
    } else {
        fprintf(stderr, "No suitable device format available.\n");
        return 1;
    }

    if ((err = soundio_outstream_open(outstream))) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return 1;
    }

    fprintf(stderr, "Software latency: %f\n", outstream->software_latency);

    if (outstream->layout_error)
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

    if ((err = soundio_outstream_start(outstream))) {
        fprintf(stderr, "unable to start device: %s\n", soundio_strerror(err));
        return 1;
    }

    return 0;
}

void destroy_audio_client() {
    soundio_outstream_destroy(outstream);
    soundio_device_unref(device);
    soundio_destroy(soundio);
}

void load_sound(std::string filename) {
    if (sounds.find(filename) != sounds.end()) {
        return;
    }
    
    auto buffer = load_ogg(filename.c_str());
    if (buffer.sample_rate == -1) {
        printf("ERR Failed to load sound: %s\n", filename.c_str());
        return;
    }
    
    sounds.insert(std::make_pair(filename, std::move(buffer)));
}

void play_sound(std::string filename) {
    if (sounds.find(filename) == sounds.end()) {
        printf("ERR Trying to play unloaded sound: %s\n", filename.c_str());
        return;
    }

    buffer_to_play = &sounds[filename];
    samples_played = 0;
    #ifdef __APPLE__
    soundio_outstream_pause(outstream, false);
    #endif
}

}
