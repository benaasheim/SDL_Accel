//
//  Triangles.h
//  SDL_Expensive
//
//  Created by Ben on 9/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Triangles_h
#define Triangles_h

#include <stdio.h>
#include <math.h>
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"
#include <SDL2/SDL.h>


#define squareff(f) (f * f)

struct __attribute__((__packed__)) Triangle {
    simd_float3 a; //center of Circle, center/position of Plane
    simd_float3 b; //xCoord is radius of Circle, normal of Plane
    simd_float3 c; //third vertex of triangle
    int mat; //photo-material reference in scene materials-list
    SDL_Surface* img;
};

typedef struct Triangle Triangle;

extern unsigned int numTriangles;
extern Triangle* triangles;


Triangle makeTri(simd_float3 a, simd_float3 b, simd_float3 c, int material, SDL_Surface* sur);
float calcTriT(const Ray ray, const Triangle* triangle);
simd_float3 calcIntersectionTri2(const simd_float3 raydirection, Triangle* triangle, const float t);
simd_float3 triImageTransform(simd_float3 point, Triangle* triangle);
void add_Triangle(Triangle object);


#endif /* Triangles_h */
