//
//  Planes.c
//  SDL_Expensive
//
//  Created by Ben on 9/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Planes.h"

#define max_planes 50
Plane _planes[max_planes];
unsigned int numPlanes = 0;
Plane* planes  = _planes;

float calcPlnT(const Ray ray, const Plane* plane, simd_float3* normal) {
    
    float d = simd_dot(plane->n, ray.direction);
    simd_float3 P = plane->c - ray.origin;
    float t;
//    if (d > 0.0f) {
//        d = simd_dot(-1 * plane->n, ray.direction);
////        d *= -1;
//        if (d > 0.0f) {
//            return -1;
//        }
//        else {
//            t = simd_dot(P, -1 * plane->n)/d;
//            *normal = -1 * plane->n;
//        }
//    }
//    else {
//        t = simd_dot(P, plane->n)/d;
//        *normal = plane->n;
//
//    }
    *normal = plane->n;
    if (d > 0.0f) {
//        d *= -1;
//        *normal *= -1;
        return -1;
    }
//
    t = simd_dot(P, *normal)/d;
    
    return t;
}

Hit calcIntersectionPln(const Ray ray, const Plane* plane, const float t) {
    Hit hit;
    hit.position = ray.origin + (ray.direction * t);
    hit.normal = plane->n;
    return hit;
}

simd_float3 calcIntersectionPln2(const Plane* plane) {
    return plane->n;
}
Plane makePln(simd_float3 center, simd_float3 radius, Material* material) {
    Plane object;
    object.c = center;
    object.n = radius;
    object.mat = material;
    object.img = NULL;
    return object;
}

void add_Plane(Plane object) {
    if (numPlanes < max_planes) {
        _planes[numPlanes++] = object;
    }
}
