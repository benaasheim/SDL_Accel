//
//  Unit.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/27/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Unit_h
#define Unit_h
#include "Vector.h"

typedef struct {
    Vector position;
    Camera camera;
    float speed;
    float sensitivity;
} Unit;

#endif /* Unit_h */
