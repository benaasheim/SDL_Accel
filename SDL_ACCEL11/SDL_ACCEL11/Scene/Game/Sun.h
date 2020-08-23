//
//  Sun.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Sun_h
#define Sun_h

#include <stdio.h>
#include <simd/simd.h>

#include "Light.h"
#include "Objects.h"

typedef struct {
    Light light;
    Object sphere;
} Sun;

Sun newSun(simd_float3 color, simd_float3 position, float intensity, float constant, float linear, float exponent, float radius, int material);


#endif /* Sun_h */
