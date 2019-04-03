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

void update() {
    
    ContextMenu::update([]() {
        Overlay::update([]() {
            ddui::fill_color(ddui::rgb(0x000000));
            ddui::font_face("mono");
            ddui::font_size(120.0);
            ddui::text_align(ddui::align::CENTER | ddui::align::MIDDLE);
            ddui::text(ddui::view.width / 2, ddui::view.height / 2, "RRREPEAT", NULL);
        });
    });

}
