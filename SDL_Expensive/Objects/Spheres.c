//
//  Spheres.c
//  SDL_Expensive
//
//  Created by Ben on 9/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Spheres.h"
#include <math.h>

#define max_spheres 50
Sphere _spheres[max_spheres];
unsigned int numSpheres = 0;
Sphere* spheres  = _spheres;

float calcSphT(const Ray ray, const Sphere* sphere) {
    simd_float3 SP;
    float t, b, d;
    
    SP = sphere->c - ray.origin;
    
    b = simd_dot(SP, ray.direction);
    
    d = b * b - simd_dot(SP, SP) + squareff(sphere->r);
    
    if (d < 0.0f) {
        return -1.0f;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    return t;
}

Hit calcIntersectionSph(const Ray ray, const Sphere* sphere, const float t) {
    Hit hit;
    hit.position = ray.origin + (ray.direction * t);
    hit.normal = (hit.position - sphere->c)/sphere->r;
    return hit;
}

simd_float3 calcIntersectionSph2(const simd_float3 position, const Sphere* sphere) {
    return (position - sphere->c)/sphere->r;
}

simd_float3 sphImageTransform3(Hit* hit, Sphere* sphere) {
    simd_float3 norm = hit->normal;
    SDL_Surface* image = sphere->img;
    Uint8 r, g, b;
    Uint32 pixel;
    float u, v;
    int i, j;
    norm = simd_normalize(norm);
    v = asinf(norm.y) / M_PI ;
    u = atan2f(norm.z, norm.x)/(2*M_PI);;
    u += 0.5;
    // u and v are between 0 and 1
    i = (int)((float)image->w * (v - ((int)v)));
    j = (int)((float)image->h * (u - ((int)u)));
    
    pixel = ((Uint32*)(image->pixels))[j * image->w + i];
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    int x = 255;
    float rr, gg, bb;
    rr = r;
    gg = g;
    bb = b;
    rr /= x;
    gg /= x;
    bb /= x;

    simd_float3 retv = (simd_float3){rr, gg, bb};
    return retv;
}

Sphere makeSph(simd_float3 center, float radius, Material* material, SDL_Surface* sur) {
    Sphere object;
    object.c = center;
    object.r = radius;
    object.mat = material;
    object.img = sur;
    return object;
}

void add_Sphere(Sphere object) {
    if (numSpheres < max_spheres) {
        _spheres[numSpheres++] = object;
    }
}
