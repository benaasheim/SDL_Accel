//
//  Light.c
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Light.h"

Light newLight(simd_float3 color, simd_float3 position, float intensity, float constant, float linear, float exponent) {
    Light light;
    
    light.position = position;
    light.color = color * intensity;
    light.constant = constant;
    light.linear = linear;
    light.exponent = exponent;
    
    return light;
}
