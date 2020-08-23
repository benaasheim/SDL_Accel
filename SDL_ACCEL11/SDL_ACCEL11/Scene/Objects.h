//
//  Objects.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Objects_h
#define Objects_h

#include <stdio.h>
#include <math.h>

#include "Hit.h"
#include "Settings.h"
#include "Materials.h"
#include "Ray.h"

#define squareff(f) (f * f)

struct __attribute__ ((__packed__)) Object {
    simd_float3 c; //center of Circle, center/position of Plane
    simd_float3 n; //xCoord is radius of Circle, normal of Plane
    simd_float3 z; //third vertex of triangle
    Hit (*interesect)(Ray ray, struct Object* obj, float t); //intersection method
    float (*gett)(Ray ray, struct Object* obj); //intersection method
    int mat; //photo-material reference in scene materials-list
};

typedef struct Object Object;

float calcPlaneT(Ray ray, struct Object* plane);
float calcSphereT(Ray ray, struct Object* sphere);
float calcTriangleT(Ray ray, struct Object* triangle);
Hit calcIntersectionPlane2(Ray ray, struct Object* plane, float t);
Hit calcIntersectionSphere2(Ray ray, struct Object* sphere, float t);
Hit calcIntersectionTriangle(Ray ray, struct Object* triangle, float t);
Object makeObject(simd_float3 center, simd_float3 p2, simd_float3 p3, Hit (*inter)(Ray ray, struct Object* obj, float t), float (*gt)(Ray ray, struct Object* obj), int material);
Object makeSphere(simd_float3 center, float radius, int material);
Object makePlane(simd_float3 center, simd_float3 radius, int material);
Object makeTriangle(simd_float3 a, simd_float3 b, simd_float3 c, int material);

#endif /* Objects_h */
