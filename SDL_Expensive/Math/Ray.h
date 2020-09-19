//
//  Ray.h
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

#include <simd/simd.h>

struct __attribute__ ((__packed__)) Ray {
    simd_float3 direction;
    simd_float3 origin;
};

typedef struct Ray Ray;

#endif /* Ray_h */
