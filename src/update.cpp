//
//  update.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 03/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include "update.hpp"

#include "globals.hpp"
#include "render_waveform.hpp"
#include <ddui/core>

using namespace ddui;

static auto ANIMATION_ID = (void*)0xA0DE;

void update() {

    float completion;
    bool running;
    {
        std::unique_lock<std::mutex> lock(global_mutex);
        if (buffer == NULL) {
            running = false;
            completion = 1.0;
        } else {
            running = playing;
            completion = (float)samples_played / (float)buffer->number_of_samples;
        }
    }

    if (running) {
        if (!animation::is_animating(ANIMATION_ID)) {
            animation::start(ANIMATION_ID);
        }
        animation::get_time_elapsed(ANIMATION_ID);
    }

    if (has_key_event() && key_state.character &&
        key_state.action == keyboard::ACTION_PRESS) {
        auto ch = key_state.character[0];
        if (ch == ' ') {
            std::unique_lock<std::mutex> lock(global_mutex);
            playing = !playing;
        }
        switch (ch) {
            case 'a': completion = 0.0; break;
            case 's': completion = 0.1; break;
            case 'd': completion = 0.2; break;
            case 'f': completion = 0.3; break;
            case 'g': completion = 0.4; break;
            case 'h': completion = 0.5; break;
            case 'j': completion = 0.6; break;
            case 'k': completion = 0.7; break;
            case 'l': completion = 0.8; break;
            case ';': completion = 0.9; break;
            default: break;
        }
        consume_key_event();
        {
            std::unique_lock<std::mutex> lock(global_mutex);
            samples_played = completion * buffer->number_of_samples;
        }
    }

    constexpr auto BLOCK_HEIGHT = 100.0;

    sub_view(0, 0, view.width, BLOCK_HEIGHT);
    {

        render_waveform();


        begin_path();
        stroke_color(rgb(0xff0000));
        move_to(view.width * completion, 0);
        line_to(view.width * completion, view.height);
        stroke();
    }
    restore();

}
