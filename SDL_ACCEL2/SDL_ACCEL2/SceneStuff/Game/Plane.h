//
//  Plane.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/27/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Plane_h
#define Plane_h
#include "Vector.h"

typedef struct {
    Vector position;
    Quaternion rot;
    float speed;
    float sensitivity;
} Plane;




#endif /* Plane_h */
