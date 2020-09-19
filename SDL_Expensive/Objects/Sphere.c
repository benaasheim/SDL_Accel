//
//  Sphere.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Object.h"
#include <math.h>

float calcSphereT(const Ray ray, const struct Object* sphere) {
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

Hit calcIntersectionSphere(const Ray ray, const struct Object* sphere, const float t) {
    Hit hit;
    hit.position = ray.origin + (ray.direction * t);
    hit.normal = (hit.position - sphere->c)/sphere->n.x;
    return hit;
}

simd_float3 calcIntersectionSphere2(const simd_float3 position, const simd_float3 p1, const struct Object* sphere, const float t) {
    return (position - sphere->c)/sphere->n.x;
}

simd_float3 sphereImageTransform(simd_float3 point, struct Object* sphere) {
    SDL_Surface* image = sphere->img;
    Uint8 r, g, b;
    Uint32 pixel;

//    y_img = 0.5 + atan2f(point.z, point.x) / (2 * M_PI);
//    x_img = 0.5 - asinf(point.y) / M_PI;
    float phi = atan2f(point.z, point.x);
    float theta = asinf(point.y);
    int u = 1 - (phi + M_PI) / (2 * M_PI);
    int v = (theta + M_PI / 2) / M_PI;
    
    pixel = ((Uint32*)(image->pixels))[u * image->w + v];
    
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    
    return (simd_float3){(float)r, (float)g, (float)b};
}

simd_float3 sphereImageTransform2(simd_float3 norm, simd_float3 point, struct Object* sphere) {
    SDL_Surface* image = sphere->img;
    Uint8 r, g, b;
    Uint32 pixel;
    
    float u = 0.5 + atan2f(norm.z, norm.x) / (2 * M_PI);
    
    float v = asinf(norm.y) / M_PI;
    
    float theta = acosf(norm.y);
    float phi = atan2f(norm.x, norm.z);
    
    if (phi < 0.0) {
        phi += 2*M_PI;
    }
    
    
    float uu = phi/(2*M_PI);
    float vv = (M_PI - theta)/M_PI;
    
    int column = (int)((image->w - 1) * u);
    int row = (int)((image->h - 1) * v);
    
//    pixel = ((Uint32*)(image->pixels))[height * image->w + width];
    pixel = ((Uint32*)(image->pixels))[row * image->w + column];
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    

    return (simd_float3){(float)r, (float)g, (float)b};
}

simd_float3 sphereImageTransform3(simd_float3 point, simd_float3 norm, struct Object* sphere) {
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
    
    
//    pixel = ((Uint32*)(image->pixels))[height * image->w + width];
    pixel = ((Uint32*)(image->pixels))[j * image->w + i];
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
//    int x = image->format->Amask;
    int x = 255;
//    r /= x;
//    g /= x;
//    b /= x;
    float rr, gg, bb;
    rr = r;
    gg = g;
    bb = b;
    rr /= x;
    gg /= x;
    bb /= x;

//    simd_float3 retv = (simd_float3){(float)r, (float)g, (float)b};
    simd_float3 retv = (simd_float3){rr, gg, bb};
    return retv;
}

Object makeSphere(simd_float3 center, float radius, int material, SDL_Surface* sur) {
    Object object;
    simd_float3 oth = {radius, 0, 0};
    simd_float3 (*inter)(const simd_float3 position, const simd_float3 raydirection,  const struct Object* triangle, const float t) = calcIntersectionSphere2;
    float (*gt)(const Ray ray, const struct Object* obj) = calcSphereT;
    object = makeObject(center, oth, oth, inter, gt, material, 's', sur);
    object.surfaceToImage = sphereImageTransform3;
    return object;
}
