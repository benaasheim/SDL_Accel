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

float calcTriT(const Ray ray, const Triangle* triangle, simd_float3* normal) {
    
    float d = simd_dot(triangle->n, ray.direction);
    *normal = triangle->n;
    float helper = 1;
    float t;
    
    if (d > 0.0f) {
        d *= -1;
        *normal *= -1;
        helper *= -1;
    }
    
    float p1 = simd_dot(*normal, ray.origin) ;
    float p2 = simd_dot(*normal, triangle->a);
    t = -(p1 - p2) / d;

    
    float denom = simd_dot(*normal, *normal);
    
    simd_float3 P = ray.direction * t + ray.origin;
    
    //prevents grainy see-through effect on relflective/refractive materials
    if (t - epsilon <= 0.0f) {
        return -1;
    }

    simd_float3 C;
    float u, v;

    // edge 0
    simd_float3 edge0 = triangle->b - triangle->a;
    simd_float3 vp0 = P - triangle->a;
    C = simd_cross(edge0, vp0);
    C *= helper;
    if (simd_dot(*normal, C) < 0 ) {
        return -1; // P is on the right side
    }
        

    // edge 1
    simd_float3 edge1 = triangle->c - triangle->b;
    simd_float3 vp1 = P - triangle->b;
    C = simd_cross(edge1, vp1);
    C *= helper;
    if ((u = simd_dot(*normal, C)) < 0 )  return -1; // P is on the right side

    // edge 2
    simd_float3 edge2 = triangle->a - triangle->c;
    simd_float3 vp2 = P - triangle->c;
    C = simd_cross(edge2, vp2);
    C *= helper;
    if ((v = simd_dot(*normal, C)) < 0 ) return -1; // P is on the right side;

    u /= denom;
    v /= denom;
    
    return t; // this ray hits the triangle
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
    hitnormal =
//    simd_normalize(simd_cross((triangle->b - triangle->a), (triangle->c - triangle->a)));
    triangle->n;

    float d = simd_dot(hitnormal, raydirection);
    if (d > 0.0f) {
        hitnormal *= -1;
    }
    return hitnormal;
}

simd_float3 triImageTransform2(Hit* hit, Triangle* triangle) {
    simd_float3 point = hit->position;
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
    
    int x_img = (d1 * ctx) + (d2 * btx) + (d3 * ctx);
    int y_img = ((d1 * cty) + (d2 * bty) + (d3 * cty));
    y_img = image->h - y_img;
    pixel = ((Uint32*)(image->pixels))[(int)(y_img * image->w + x_img)];
    
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    
    return (simd_float3){(float)r, (float)g, (float)b};
}

simd_float3 triImageTransform4(Hit* hit, Triangle* triangle) {
    simd_float3 point = hit->position;
    SDL_Surface* image = triangle->img;
    Uint8 r, g, b;
    Uint32 pixel;

    simd_float3 N = hit->normal;
    float denom = simd_dot(N, N);
    int mm = 1;

    simd_float2 A = {0, 0}, B = {0, 35 * mm}, C = {20 * mm, 0}, PP = {0, 0};
    
    
    simd_float3 P = hit->position;
    
    simd_float3 CC;
    float u, v;
    
    // edge 0
//    simd_float3 edge0 = triangle->b - triangle->a;
//    simd_float3 vp0 = P - triangle->a;
//    C = simd_cross(edge0, vp0);
//    if (simd_dot(N, C) < 0) return -1; // P is on the right side

    // edge 1
    simd_float3 edge1 = triangle->c - triangle->b;
    simd_float3 vp1 = P - triangle->b;
    CC = simd_cross(edge1, vp1);

    if ((u = simd_dot(N, CC)) < 0) {
        return -1; // P is on the right side

    }

    // edge 2
    simd_float3 edge2 = triangle->a - triangle->c;
    simd_float3 vp2 = P - triangle->c;
    CC = simd_cross(edge2, vp2);
    if ((v = simd_dot(N, CC)) < 0) {
        return -1; // P is on the right side;

    }

    u /= denom;
    v /= denom;
    
//    PP = u*aa + v*cc + (1-u-v)*bb;
//    PP = u * (cc - bb)  +  v * (aa - cc) + aa;

    if (PP.x > 0) {
        
    }
    PP.y = image->h - PP.y;
    
    float AreaABC = denom;
    
    float AreaPBC = simd_dot(N, simd_cross(triangle->b - P, triangle->c - P));
    float aa = AreaPBC / AreaABC;
    
    float AreaPCA = simd_dot(N, simd_cross(triangle->c - P, triangle->a - P));
    float bb = AreaPCA / AreaABC;
    
    float cc = 1.0f - aa - bb;
    
    PP = aa*A + bb*B + cc*C;
    
    PP.y = image->h - PP.y;
    pixel = ((Uint32*)(image->pixels))[(int)(PP.y * image->w + PP.x)];
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    
//
    return (simd_float3){(float)r, (float)g, (float)b};
    
//    simd_float3 cols[3] = {{0.6, 0.4, 0.1}, {0.1, 0.5, 0.3}, {0.1, 0.3, 0.7}};
//    simd_float3 r = {1, 0, 0}, g = {0, 1, 0}, b = {0, 0, 1};
//    return u * r + v * g + (1-u-v) * b;
//    return  u * cols[0] + v * cols[1] + (1 - u - v) * cols[2];
}

simd_float3 triImageTransform1(Hit* hit, Triangle* triangle) {
    simd_float3 point = hit->position;
    SDL_Surface* image = triangle->img;
    Uint8 r, g, b;
    Uint32 pixel;

    simd_float3 N = hit->normal;
//    float denom = simd_dot(N, N);
    int mm = 1;

    simd_float2 A = {0, 0}, B = {0, 35 * mm}, C = {20 * mm, 0}, PP = {0, 0};
    
    
    simd_float3 P = hit->position;
    float u, v, w;
    
    simd_float3 v0 = triangle->b-triangle->a, v1 = triangle->c-triangle->a, v2 = P-triangle->a;
    
//    simd_float3 u = triangle->b - triangle->a;
//    simd_float3 v = triangle->b - triangle->a;
//    simd_float3 w = triangle->a - ray.origin;
//    w = ray.direction * t - w;
    float d00 = simd_dot(v0, v0);
    float d01 = simd_dot(v0, v1);
    float d11 = simd_dot(v1, v1);
    float d20 = simd_dot(v2, v0);
    float d21 = simd_dot(v2, v1);
    float denom = d00*d11 - d01*d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    
    PP = v * A + w * C + (1 - v - w) * B;
    
//    PP.y = image->h - PP.y;
    pixel = ((Uint32*)(image->pixels))[(int)(PP.y * image->w + PP.x)];
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    
    return (simd_float3){(float)r, (float)g, (float)b};
}


Triangle makeTri(simd_float3 a, simd_float3 b, simd_float3 c, Material* material, SDL_Surface* sur) {
    Triangle object;
    object.a = a;
    object.b = b;
    object.c = c;
    object.n = simd_normalize(simd_cross((object.b - object.a), (object.c - object.a)));
//    printf("TN: %f %f %f \n", object.n.x, object.n.y, object.n.z);
    object.m = (a + b + c)/3;
    object.mat = material;
    object.img = sur;
    return object;
}

void add_Triangle(Triangle object) {
    if (numTriangles < max_triangles) {
        _triangles[numTriangles++] = object;
    }
}


