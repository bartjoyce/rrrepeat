//
//  update.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 03/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include "update.hpp"

#include "globals.hpp"
#include <ddui/views/ContextMenu>
#include <ddui/views/Overlay>

using namespace ddui;

void update() {

    static int image = -1;
    if (image == -1) {
        int w = 100;
        int h = 100;

        auto fg_color = rgb(0x334455);
        auto bg_color = rgba(0x000000, 0);
        unsigned char fg_bytes[4];
        unsigned char bg_bytes[4];

        fg_bytes[0] = (unsigned char)(fg_color.r * 255.0f);
        fg_bytes[1] = (unsigned char)(fg_color.g * 255.0f);
        fg_bytes[2] = (unsigned char)(fg_color.b * 255.0f);
        fg_bytes[3] = (unsigned char)(fg_color.a * 255.0f);
        bg_bytes[0] = (unsigned char)(bg_color.r * 255.0f);
        bg_bytes[1] = (unsigned char)(bg_color.g * 255.0f);
        bg_bytes[2] = (unsigned char)(bg_color.b * 255.0f);
        bg_bytes[3] = (unsigned char)(bg_color.a * 255.0f);

        unsigned char data[4 * w * h];
        for (auto x = 0; x < w; ++x) {
            auto value = (x + 20) % (h / 2);
            for (auto y = 0; y < h; ++y) {
                auto i = 4 * (w * y + x);
                auto color = ((y < h / 2) ? (y > h / 2 - value) : (y < h / 2 + value)) ? fg_bytes : bg_bytes;
                data[i  ] = color[0];
                data[i+1] = color[1];
                data[i+2] = color[2];
                data[i+3] = color[3];
            }
        }
        image = create_image_from_rgba(100, 100, image::NEAREST, data);
    }

    ContextMenu::update([]() {
        Overlay::update([]() {

            auto w = 100;
            auto h = 100;
            auto x = 0.5 * (view.width - w);
            auto y = 0.5 * (view.height - h);

            auto paint = image_pattern(x, y, w, h, 0.0f, image, 1.0f);
            begin_path();
            rounded_rect(x, y, w, h, 4);
            fill_paint(paint);
            fill();
        });
    });

}
