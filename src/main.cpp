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
#include "audio_client.hpp"
#include "load_ogg.hpp"
#include "globals.hpp"
#include "write_callback.hpp"

int main(int argc, const char** argv) {

    // ddui (graphics and UI system)
    if (!ddui::app_init(700, 300, "", update)) {
        printf("Failed to init ddui.\n");
        return 0;
    }
    
    // Load the rhodes sound
    buffer = new StereoBuffer;
    *buffer = load_ogg("rhodes.ogg");

    // Type faces
    ddui::create_font("regular", "SFRegular.ttf");
    ddui::create_font("medium", "SFMedium.ttf");
    ddui::create_font("bold", "SFBold.ttf");
    ddui::create_font("thin", "SFThin.ttf");
    ddui::create_font("mono", "PTMono.ttf");

    // Audio client
    if (init_audio_client(write_callback)) {
        printf("Failed to init audio client.\n");
        return 0;
    }

    ddui::app_run();

    destroy_audio_client();
    
    return 0;
}
