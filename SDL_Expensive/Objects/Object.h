//
//  Object.h
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Object_h
#define Object_h

#include <stdio.h>
#include <math.h>
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"
#include <SDL2/SDL.h>


#define squareff(f) (f * f)

struct __attribute__((__packed__)) Object {
    simd_float3 c; //center of Circle, center/position of Plane
    simd_float3 n; //xCoord is radius of Circle, normal of Plane
    simd_float3 z; //third vertex of triangle
    simd_float3 (*interesect)(const simd_float3 A, const simd_float3 B, const struct Object* obj, const float t); //intersection method
    float (*gett)(const Ray ray, const struct Object* obj); //intersection method
    int mat; //photo-material reference in scene materials-list
    simd_float3 (*surfaceToImage)(simd_float3 point, simd_float3 norm, struct Object* obj);
    SDL_Surface* img;
};

typedef struct Object Object;

//universal functions

Object makeObject(simd_float3 center, simd_float3 p2, simd_float3 p3, simd_float3 (*inter)(const simd_float3 position, const simd_float3 raydirection, const struct Object* triangle, const float t), float (*gt)(const Ray ray, const struct Object* obj), int material, char type, SDL_Surface* sur);

//simd_float3 surfaceToImage(simd_float3 point, Object* obj);


//shape specific

Object makePlane(simd_float3 center, simd_float3 radius, int material);
//float calcPlaneT(const Ray ray, const struct Object* plane);
//Hit calcIntersectionPlane(const Ray ray, const struct Object* plane, const float t);
//simd_float3 calcIntersectionPlane2(const simd_float3 position, const simd_float3 p1,  const struct Object* plane, const float t);

Object makeSphere(simd_float3 center, float radius, int material, SDL_Surface* sur);
//float calcSphereT(const Ray ray, const struct Object* sphere);
//Hit calcIntersectionSphere(const Ray ray, const struct Object* sphere, const float t);
//simd_float3 calcIntersectionSphere2(const simd_float3 position, const simd_float3 p1, const struct Object* sphere, const float t);

Object makeTriangle(simd_float3 a, simd_float3 b, simd_float3 c, int material, SDL_Surface* sur);
//float calcTriangleT(const Ray ray, const struct Object* triangle);
//Hit calcIntersectionTriangle(const Ray ray, const struct Object* triangle, const float t);
//simd_float3 calcIntersectionTriangle2(const simd_float3 waste, const simd_float3 raydirection,  const struct Object* triangle, const float t);

void findPointsatDists(float x1, float x2, float * x4a, float * x4b, float y1, float y2, float * y4a, float * y4b, float d1, float d2);
#endif /* Object_h */

