//
//  write_callback.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 05/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include "write_callback.hpp"
#include "globals.hpp"

void write_callback(int num_samples, std::function<void(float, float)> write_sample) {
    std::unique_lock<std::mutex> lock(global_mutex);

    if (!playing || buffer == NULL) {
        for (int i = 0; i < num_samples; ++i) {
            write_sample(0.0, 0.0);
        }
        return;
    }

    int samples_left = buffer->number_of_samples - samples_played;
    int num_frames = num_samples < samples_left ? num_samples : samples_left;

    auto buffer_ptr = buffer->samples.get() + (samples_played * 2);
    for (int i = 0; i < num_frames; ++i) {
        write_sample(buffer_ptr[0], buffer_ptr[1]);
        buffer_ptr += 2;
    }
    for (int i = num_frames; i < num_samples; ++i) {
        write_sample(0.0, 0.0);
    }

    samples_played += num_frames;
    if (samples_left <= num_frames) {
//        buffer = NULL;
        samples_played = 0;
    }
}
