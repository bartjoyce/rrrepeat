//
//  main.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 03/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include <ddui/core>
#include <ddui/app>
#include "update.hpp"
#include "sound/sound.hpp"

int main(int argc, const char** argv) {

    // ddui (graphics and UI system)
    if (!ddui::app_init(300, 300, "", update)) {
        printf("Failed to init ddui.\n");
        return 0;
    }
    
    // Sound
    if (sound::init_audio_client()) {
        printf("Failed to init audio client.\n");
        return 0;
    }
    sound::load_sound("rhodes.ogg");
    sound::play_sound("rhodes.ogg");

    // Type faces
    ddui::create_font("regular", "SFRegular.ttf");
    ddui::create_font("medium", "SFMedium.ttf");
    ddui::create_font("bold", "SFBold.ttf");
    ddui::create_font("thin", "SFThin.ttf");
    ddui::create_font("mono", "PTMono.ttf");

    ddui::app_run();

    sound::destroy_audio_client();
    
    return 0;
}
