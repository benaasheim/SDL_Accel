//
//  Hit.h
//  SDL_ACCEL8
//
//  Created by Ben on 8/11/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Hit_h
#define Hit_h

#include "Vector.h"

typedef struct {
    void* obj[SIZE];
    vec normal;
    vec position;
    float t[SIZE];
} Hit;

#endif /* Hit_h */
