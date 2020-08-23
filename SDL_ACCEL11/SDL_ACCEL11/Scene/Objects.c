//
//  Objects.c
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Objects.h"
float calcPlaneT(Ray ray, struct Object* plane) {
    
    float d = simd_dot(plane->n, ray.direction);
    
    if (d > 0.0f) {
        return -1.0f;
    }
        
    simd_float3 P = plane->c - ray.origin;
    
    float t = simd_dot(P, plane->n)/d;
    
    return t;
}

Hit calcIntersectionPlane2(Ray ray, struct Object* plane, float t) {
    Hit hit;
    hit.position = ray.origin + (ray.direction * t);
    hit.normal = plane->n;
    return hit;
}

float calcSphereT(Ray ray, struct Object* sphere) {
    simd_float3 SP;
    float t, b, d;
    
    SP = sphere->c - ray.origin;
    
    b = simd_dot(SP, ray.direction);
    
    d = b * b - simd_dot(SP, SP) + squareff(sphere->n.x);
    
    if (d < 0.0f) {
        return -1.0f;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    return t;
}

Hit calcIntersectionSphere2(Ray ray, struct Object* sphere, float t) {
    Hit hit;
    hit.position = ray.origin + (ray.direction * t);
    hit.normal = (hit.position - sphere->c)/sphere->n.x;
    return hit;
}



Object makeObject(simd_float3 center, simd_float3 p2, simd_float3 p3, Hit (*inter)(Ray ray, struct Object* obj, float t), float (*gt)(Ray ray, struct Object* obj), int material) {
    Object object;
    object.c = center;
    object.n = p2;
    object.z = p3;
    object.interesect = inter;
    object.gett = gt;
    object.mat = material;
    return object;
}

Object makeSphere(simd_float3 center, float radius, int material) {
    Object object;
    simd_float3 oth = {radius, 0, 0};
    Hit (*inter)(Ray ray, struct Object* obj, float t) = calcIntersectionSphere2;
    float (*gt)(Ray ray, struct Object* obj) = calcSphereT;
    object = makeObject(center, oth, oth, inter, gt, material);
    return object;
}

Object makePlane(simd_float3 center, simd_float3 radius, int material) {
    Object object;
    Hit (*inter)(Ray ray, struct Object* obj, float t) = calcIntersectionPlane2;
    float (*gt)(Ray ray, struct Object* obj) = calcPlaneT;
    simd_float3 oth = {0, 0, 0};
    object = makeObject(center, radius, oth, inter, gt, material);
    return object;
}

float calcTriangleT(Ray ray, struct Object* triangle) {
    
    simd_float3 plane1n = simd_normalize(simd_cross((triangle->n - triangle->c), (triangle->z - triangle->c)));
    
    
    float d = simd_dot(plane1n, ray.direction);
    if (d > 0.0f) {
        plane1n *= -1;
        d *= -1;
    }
    float t = simd_dot(triangle->c - ray.origin, plane1n)/d;
    
    simd_float3 u = triangle->n - triangle->c;
    simd_float3 v = triangle->z - triangle->c;
    simd_float3 w = (ray.origin + (ray.direction * t)) - triangle->c;
    
    float uDv = simd_dot(u, v);
    float wDv = simd_dot(w, v);
    float vDv = simd_dot(v, v);
    float wDu = simd_dot(w, u);
    float uDu = simd_dot(u, u);
    
    float s1 = (uDv*wDv - vDv*wDu)/(uDv*uDv - uDu*vDv);
    float t1 = (uDv*wDu - uDu*wDv)/(uDv*uDv - uDu*vDv);
    
    if (s1 >= 0.0f && t1 >= 0.0f && s1 + t1 < 1.0f) {
        return t;
    }
    return -1.0;
}

Hit calcIntersectionTriangle(Ray ray, struct Object* triangle, float t) {
    simd_float3 plane1n = simd_normalize(simd_cross((triangle->n - triangle->c), (triangle->z - triangle->c)));
    
    Hit hit;
    
    float d = simd_dot(plane1n, ray.direction);
    if (d > 0.0f) {
        plane1n *= -1;
    }
    hit.position = ray.origin + (ray.direction * t);
    hit.normal = plane1n;
    return hit;
}

Object makeTriangle(simd_float3 a, simd_float3 b, simd_float3 c, int material) {
    Object object;
    Hit (*inter)(Ray ray, struct Object* obj, float t) = calcIntersectionTriangle;
    float (*gt)(Ray ray, struct Object* obj) = calcTriangleT;
    object = makeObject(a, b, c, inter, gt, material);
    return object;
}
