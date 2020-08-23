//
//  Ray.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

#include <stdio.h>
#include <simd/simd.h>

struct __attribute__ ((__packed__)) Ray{
    simd_float3 direction;
    simd_float3 origin;
};

typedef struct Ray Ray;

void printVec(simd_float3 vec);

#endif /* Ray_h */
