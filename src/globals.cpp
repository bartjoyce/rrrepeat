//
//  globals.cpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 03/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#include "globals.hpp"

std::mutex global_mutex;
StereoBuffer* buffer = NULL;
long samples_played = 0;
bool playing = true;
// KeyState key_states[12];
// long current_time = 0;
