//
//  Ray.h
//  SDL_Accel
//
//  Created by Ben on 7/22/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Ray_h
#define Ray_h
#include "Vector.h"

typedef struct {
    Vector direction;
    Vector origin;
} Ray;

Ray newRay(Vector d, Vector o) {
    Ray ray;
    ray.direction = normalize(d);
    ray.origin = o;
    return ray;
}

#endif /* Ray_h */
