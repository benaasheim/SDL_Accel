//
//  Sun.h
//  SDL_ACCEL3
//
//  Created by Ben on 7/28/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Sun_h
#define Sun_h
#include "Vector.h"
#include "Light.h"
#include "Object.h"

typedef struct {
    Light light;
    Object sphere;
} Sun;

Sun newSun(Vector color, Vector position, float intensity, float constant, float linear, float exponent, float radius, int material) {
    Sun sun;
    
    sun.light = newLight(color, position, intensity, constant, linear, exponent);
    sun.sphere = makeSphere(position, radius, material);
    
    return sun;
}


#endif /* Sun_h */
