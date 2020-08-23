//
//  Sun.c
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Sun.h"

Sun newSun(simd_float3 color, simd_float3 position, float intensity, float constant, float linear, float exponent, float radius, int material) {
    Sun sun;
    
    sun.light = newLight(color, position, intensity, constant, linear, exponent);
    sun.sphere = makeSphere(position, radius, material);
    
    return sun;
}
