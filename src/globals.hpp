//
//  globals.hpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 03/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#ifndef globals_hpp
#define globals_hpp

#include <mutex>
#include "StereoBuffer.hpp"

// struct KeyState {
//     double in_point = 0.0;
//     double out_point = 1.0;
//     double drift = 0.5;
//     double decay = 1.0;
//     bool pressed = false;
//     long press_start_time = 0;
// };

extern std::mutex global_mutex;
extern StereoBuffer* buffer;
extern long samples_played;
extern bool playing;
// extern KeyState key_states[12];
// extern long current_time;

#endif
