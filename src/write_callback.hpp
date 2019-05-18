//
//  write_callback.hpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 05/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#ifndef write_callback_hpp
#define write_callback_hpp

#include <functional>

void write_callback(int num_samples, std::function<void(float, float)> write_sample);

#endif
