//
//  Light.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include <simd/simd.h>

struct __attribute__ ((__packed__)) Light {
    simd_float3 position;
    simd_float3 color;
    float constant;
    float linear;
    float exponent;
};

typedef struct Light Light;
Light newLight(simd_float3 color, simd_float3 position, float intensity, float constant, float linear, float exponent);

#endif /* Light_h */
