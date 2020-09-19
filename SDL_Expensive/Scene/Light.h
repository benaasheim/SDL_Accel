//
//  Light.h
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include <stdio.h>
#include <simd/simd.h>

#define POINT 0
#define DIRECTIONAL 1

struct __attribute__ ((__packed__)) Light {
    simd_float3 color;
    simd_float3 position;
    simd_float3 direction;
    float intensity;
    float constant;
    float linear;
    float exponent;
    int type;
};

typedef struct Light Light;

#endif /* Light_h */
