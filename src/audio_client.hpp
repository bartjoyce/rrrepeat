//
//  audio_client.hpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 05/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#ifndef audio_client_hpp
#define audio_client_hpp

#include <soundio/soundio.h>
#include <functional>

int init_audio_client(std::function<void(int num_samples, std::function<void(float, float)> write_sample)> write_callback);
void destroy_audio_client();

#endif
