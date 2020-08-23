//
//  Hit.h
//  SDL_Accel
//
//  Created by Ben on 7/21/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Hit_h
#define Hit_h
#include "Vector.h"

typedef struct {
    void* obj;
    Vector normal;
    Vector position;
    float t;
} Hit;

#endif /* Hit_h */
