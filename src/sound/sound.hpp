//
//  sound.hpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 03/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#ifndef sound_hpp
#define sound_hpp

#include <string>

namespace sound {

int init_audio_client();
void destroy_audio_client();

void load_sound(std::string filename);
void play_sound(std::string filename);

}

#endif
