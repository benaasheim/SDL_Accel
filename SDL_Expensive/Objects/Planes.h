//
//  Planes.h
//  SDL_Expensive
//
//  Created by Ben on 9/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Planes_h
#define Planes_h


#include <stdio.h>
#include <math.h>
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"
#include "Material.h"
#include <SDL2/SDL.h>


#define squareff(f) (f * f)

struct __attribute__((__packed__)) Plane {
    simd_float3 c; //center of Circle, center/position of Plane
    simd_float3 n; //xCoord is radius of Circle, normal of Plane
    Material* mat; //photo-material reference in scene materials-list
    SDL_Surface* img;
};

typedef struct Plane Plane;

//universal functions
extern unsigned int numPlanes;
extern Plane* planes;


float calcPlnT(const Ray ray, const Plane* plane, simd_float3* normal);
simd_float3 calcIntersectionPln2(const Plane* plane);
Plane makePln(simd_float3 center, simd_float3 radius, Material* material);
void add_Plane(Plane object);

#endif /* Planes_h */
