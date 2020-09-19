//
//  Spheres.h
//  SDL_Expensive
//
//  Created by Ben on 9/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Spheres_h
#define Spheres_h

#include <stdio.h>
#include <math.h>
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"
#include <SDL2/SDL.h>


#define squareff(f) (f * f)

struct __attribute__((__packed__)) Sphere {
    simd_float3 c; //center
    float r; //radius
    int mat; //photo-material reference in scene materials-list
    SDL_Surface* img;
};

typedef struct Sphere Sphere;

extern unsigned int numSpheres;
extern Sphere* spheres;

Sphere makeSph(simd_float3 center, float radius, int material, SDL_Surface* sur);
float calcSphT(const Ray ray, const Sphere* sphere);
simd_float3 calcIntersectionSph2(const simd_float3 position, const Sphere* sphere);
simd_float3 sphImageTransform3(simd_float3 point, simd_float3 norm, Sphere* sphere);
void add_Sphere(Sphere object);

#endif /* Spheres_h */
