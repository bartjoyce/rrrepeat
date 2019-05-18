//
//  render_waveform.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 05/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include "render_waveform.hpp"
#include "globals.hpp"
#include <ddui/core>

using namespace ddui;

static int create_peak_image(int width, int height);
static void color_to_bytes(Color in, unsigned char* out);

static int image_id = -1;
static int image_width = -1;
static int image_height = -1;
static float last_view_width = 0.0;
static float last_view_height = 0.0;
static int timeout_id = -1;

void render_waveform() {
    
    if (image_id == -1) {
        image_width = 2 * view.width;
        image_height = 2 * view.height;
        last_view_width = view.width;
        last_view_height = view.height;
        image_id = create_peak_image(image_width, image_height);
    }

    auto size_changed = (
        view.width  != last_view_width ||
        view.height != last_view_height
    );
    if (size_changed) {
        timer::clear_timeout(timeout_id);
        timeout_id = timer::set_timeout([]() {
            image_id = -1;
        }, 100);
    }
    last_view_width = view.width;
    last_view_height = view.height;

    auto paint = image_pattern(0, 0, view.width, view.height, 0.0f, image_id, 1.0f);
    begin_path();
    rect(0, 0, view.width, view.height);
    fill_paint(paint);
    fill();
}

int create_peak_image(int width, int height) {
    std::unique_lock<std::mutex> lock(global_mutex);

    unsigned char fg_bytes[4];
    unsigned char bg_bytes[4];
    color_to_bytes(rgb(0x999999), fg_bytes);
    color_to_bytes(rgb(0xdddddd), bg_bytes);

    auto data = new unsigned char[4 * width * height];
    
    // Fill with background
    for (long i = 0; i < 4 * width * height; i += 4) {
        data[i  ] = bg_bytes[0];
        data[i+1] = bg_bytes[1];
        data[i+2] = bg_bytes[2];
        data[i+3] = bg_bytes[3];
    }
    
    // Fill foreground
    for (auto x = 0; x < width; ++x) {
    
        // Get the value
        int y0, y1;
        {
            auto i0 = (long)(((float)x / (float)width) * buffer->number_of_samples);
            auto i1 = (long)(((float)(x + 1) / (float)width) * buffer->number_of_samples);
            float min_value = 0.0;
            float max_value = 0.0;
            auto ptr_begin = buffer->samples.get() + i0 * 2;
            auto ptr_end = buffer->samples.get() + i1 * 2;
            for (auto ptr = ptr_begin; ptr < ptr_end; ++ptr) {
                if (min_value > *ptr) {
                    min_value = *ptr;
                }
                if (max_value < *ptr) {
                    max_value = *ptr;
                }
            }
            y0 = height * (1.0 - (max_value + 1.0) * 0.5);
            y1 = height * (1.0 - (min_value + 1.0) * 0.5);
        }
        
        // Fill in the pixels
        for (auto y = y0; y <= y1; ++y) {
            auto i = 4 * (width * y + x);
            data[i  ] = fg_bytes[0];
            data[i+1] = fg_bytes[1];
            data[i+2] = fg_bytes[2];
            data[i+3] = fg_bytes[3];
        }
    }

     return create_image_from_rgba(width, height, 0, data);
}

static void color_to_bytes(Color in, unsigned char* out) {
    out[0] = (unsigned char)(in.r * 255.0f);
    out[1] = (unsigned char)(in.g * 255.0f);
    out[2] = (unsigned char)(in.b * 255.0f);
    out[3] = (unsigned char)(in.a * 255.0f);
}
