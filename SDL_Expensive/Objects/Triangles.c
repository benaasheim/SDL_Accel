//
//  Triangles.c
//  SDL_Expensive
//
//  Created by Ben on 9/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Triangles.h"

#define max_triangles 50
Triangle _triangles[max_triangles];
unsigned int numTriangles = 0;
Triangle* triangles  = _triangles;

float calcTriT(const Ray ray, const Triangle* triangle) {
    
    simd_float3 u = triangle->b - triangle->a;
    simd_float3 v = triangle->c - triangle->a;
    simd_float3 w = triangle->a - ray.origin;
    simd_float3 plane1n = simd_normalize(simd_cross(u, v));
    
    float d = simd_dot(plane1n, ray.direction);
    
    if (d > 0.0f) {
        plane1n *= -1;
        d *= -1;
    }
    
    float t = simd_dot(w, plane1n)/d;
    
    w = ray.direction * t - w;
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

Hit calcIntersectionTri(const Ray ray, const Triangle* triangle, const float t) {
    Hit hit;
    hit.normal = simd_normalize(simd_cross((triangle->b - triangle->a), (triangle->c - triangle->a)));
    float d = simd_dot(hit.normal, ray.direction);
    if (d > 0.0f) {
        hit.normal *= -1;
    }
    hit.position = ray.origin + (ray.direction * t);
    return hit;
}

simd_float3 calcIntersectionTri2(const simd_float3 raydirection, Triangle* triangle, const float t) {
    simd_float3 hitnormal;
    hitnormal = simd_normalize(simd_cross((triangle->b - triangle->a), (triangle->c - triangle->a)));
    float d = simd_dot(hitnormal, raydirection);
    if (d > 0.0f) {
        hitnormal *= -1;
    }
    return hitnormal;
}

simd_float3 triImageTransform(simd_float3 point, Triangle* triangle) {
    SDL_Surface* image = triangle->img;
    Uint8 r, g, b;
    Uint32 pixel;
    
    float d1 = simd_length(point - triangle->a);
    float d2 = simd_length(point - triangle->b);
    float d3 = simd_length(point - triangle->c);


    float r1 = 1/ ( 1 + (d1/d2) + (d1/d3) );
    float r2 = 1/ ( (d2/d1) + 1 + (d2/d3) );
    float r3 = 1/ ( (d3/d1) + (d3/d2) + 1 );
    
    int x = r1 + r2 + r3;
    
    int atx, aty, btx, bty, ctx, cty;
    int mm = 1;
    atx = 0;
    aty = 35 * mm;

    btx = 0;
    bty = 0;
    
    ctx = 20 * mm;
    cty = 35 * mm;
    
    int x_img = (d1 * atx) + (d2 * btx) + (d3 * ctx);
    int y_img = (d1 * aty) + (d2 * bty) + (d3 * cty);
    
    pixel = ((Uint32*)(image->pixels))[(int)(y_img * image->w + x_img)];
    
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    
    return (simd_float3){(float)r, (float)g, (float)b};
}


Triangle makeTri(simd_float3 a, simd_float3 b, simd_float3 c, int material, SDL_Surface* sur) {
    Triangle object;
    object.a = a;
    object.b = b;
    object.c = c;
    object.mat = material;
    object.img = sur;
    return object;
}

void add_Triangle(Triangle object) {
    if (numTriangles < max_triangles) {
        _triangles[numTriangles++] = object;
    }
}
