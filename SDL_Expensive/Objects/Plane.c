//
//  Plane.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include <stdio.h>

#include "Object.h"

float calcPlaneT(const Ray ray, const struct Object* plane) {
    
    float d = simd_dot(plane->n, ray.direction);
    
    if (d > 0.0f) {
        return -1.0f;
    }
    
    simd_float3 P = plane->c - ray.origin;
    
    float t = simd_dot(P, plane->n)/d;
    
    return t;
}

Hit calcIntersectionPlane(const Ray ray, const struct Object* plane, const float t) {
    Hit hit;
    hit.position = ray.origin + (ray.direction * t);
    hit.normal = plane->n;
    return hit;
}

simd_float3 calcIntersectionPlane2(const simd_float3 position, const simd_float3 p1,  const struct Object* plane, const float t) {
    return plane->n;
}

Object makePlane(simd_float3 center, simd_float3 radius, int material) {
    Object object;
    simd_float3 (*inter)(const simd_float3 position, const simd_float3 raydirection,  const struct Object* triangle, const float t) = calcIntersectionPlane2;
    float (*gt)(const Ray ray, const struct Object* obj) = calcPlaneT;
    simd_float3 oth = {0, 0, 0};
    object = makeObject(center, radius, oth, inter, gt, material, 'p', NULL);
    return object;
}
