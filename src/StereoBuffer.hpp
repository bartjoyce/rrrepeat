//
//  StereoBuffer.hpp
//  RRREPEAT
//
//  Created by Bartholomew Joyce on 05/04/2019.
//  Copyright © 2019 Bartholomew Joyce All rights reserved.
//

#ifndef StereoBuffer_hpp
#define StereoBuffer_hpp

#include <memory>

struct StereoBuffer {
    int sample_rate;
    int number_of_samples;
    std::unique_ptr<float[]> samples;
};

#endif
