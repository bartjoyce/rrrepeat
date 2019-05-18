//
//  load_ogg.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 05/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include "load_ogg.hpp"
#include <ddui/util/get_asset_filename>

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

StereoBuffer load_ogg(const char* filename) {
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
