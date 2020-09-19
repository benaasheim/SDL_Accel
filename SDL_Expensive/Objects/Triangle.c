//
//  Triangle.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Object.h"

float calcTriangleT(const Ray ray, const struct Object* triangle) {
    
    simd_float3 u = triangle->n - triangle->c;
    simd_float3 v = triangle->z - triangle->c;
    simd_float3 w = triangle->c - ray.origin;
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

Hit calcIntersectionTriangle(const Ray ray, const struct Object* triangle, const float t) {
    Hit hit;
    hit.normal = simd_normalize(simd_cross((triangle->n - triangle->c), (triangle->z - triangle->c)));
    float d = simd_dot(hit.normal, ray.direction);
    if (d > 0.0f) {
        hit.normal *= -1;
    }
    hit.position = ray.origin + (ray.direction * t);
    return hit;
}

simd_float3 calcIntersectionTriangle2(const simd_float3 waste, const simd_float3 raydirection,  const struct Object* triangle, const float t) {
    simd_float3 hitnormal;
    hitnormal = simd_normalize(simd_cross((triangle->n - triangle->c), (triangle->z - triangle->c)));
    float d = simd_dot(hitnormal, raydirection);
    if (d > 0.0f) {
        hitnormal *= -1;
    }
    return hitnormal;
}

simd_float3 triangeImageTransform(simd_float3 point, struct Object* triangle) {
    SDL_Surface* image = triangle->img;
    Uint8 r, g, b;
    Uint32 pixel;
    
    float d1 = simd_length(point - triangle->c);
    float d2 = simd_length(point - triangle->n);
    float d3 = simd_length(point - triangle->z);


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

simd_float3 triangeImageTransform2(simd_float3 point, struct Object* triangle) {
    SDL_Surface* image = triangle->img;
    Uint8 r, g, b;
    Uint32 pixel;
    
    float x1, x2, x3, x4 = 0.0, y1, y2, y3, y4 = 0.0, d1, d2, d3;
    float x4_12a, x4_12b, x4_13a, x4_13b, y4_12a, y4_12b, y4_13a, y4_13b, x4_23a, x4_23b, y4_23a, y4_23b;
    
    float sc2 = 10;
    x1 = 0; y1 = 0;
    x2 = 20 * sc2; y2 = 0;
    x3 = 0; y3 = 35 * sc2;
    
    float scale = simd_length(triangle->z - triangle->c);
    float scale2 = sqrtf(fabsf((x2 - x1) + (y2 - y1)));
//    printf("scale: %f\n", scale2);
    scale =     scale/ scale2;
//    printf("scale: %f\n", scale);

    d1 = fabsf(simd_length(triangle->c - point)/scale);
    d2 = fabsf(simd_length(triangle->z - point)/scale);
    d3 = fabsf(simd_length(triangle->n - point)/scale);
    
    findPointsatDists(x1, x2, &x4_12a, &x4_12b, y1, y2, &y4_12a, &y4_12b, d1, d2);
    findPointsatDists(x1, x3, &x4_13a, &x4_13b, y1, y3, &y4_13a, &y4_13b, d1, d3);
    findPointsatDists(x2, x3, &x4_23a, &x4_23b, y2, y3, &y4_23a, &y4_23b, d2, d3);
    if (x4_12a != x4_13a &&
        x4_12a != x4_23a &&
        x4_12a != x4_13b &&
        x4_12a != x4_23b &&
        
        x4_13a != x4_23a &&
        
        x4_13a != x4_23b &&
        
        
        x4_12b != x4_13a &&
        x4_12b != x4_13b &&
        x4_12b != x4_23b &&
        x4_12b != x4_23a &&
        
        x4_13b != x4_23b
        &&
        
        
        
        x4_13b != x4_23a &&
        
        y4_12a != y4_13a &&
        y4_12a != y4_23a &&
        y4_12a != y4_13b &&
        y4_12a != y4_23b &&
        
        y4_13a != y4_23a &&
        
        y4_13a != y4_23b &&
        
        
        y4_12b != y4_13a &&
        y4_12b != y4_13b &&
        y4_12b != y4_23b &&
        y4_12b != y4_23a &&
        
        y4_13b != y4_23b
        &&
        
        
        
        y4_13b != y4_23a
        ) {
        
    }
    else {
        printf("UH OH\n");
    }
    

    
    if (x4_12a == x4_13a && y4_12a == y4_13a) {
        x4 = x4_13a;
        y4 = y4_13a;
        printf("Not HERE\n");
    }
    else if (x4_12a == x4_13b && y4_12a == y4_13b) {
        x4 = x4_13b;
        y4 = y4_13b;
        printf("Not HERE\n");
    }
    else if (x4_12b == x4_13b && y4_12b == y4_13b) {
        x4 = x4_13b;
        y4 = y4_13b;
        printf("Not HERE\n");
    }
    else if (x4_12b == x4_13a && y4_12b == y4_13a) {
        x4 = x4_13a;
        y4 = y4_13a;
        printf("Not HERE\n");
    }
    else {
//        printf("HERE\n");
    }
    
//    x4 = image->w - x4;
    y4 = image->h - y4;
    if (y4 != 124) {
            printf("x: %f  y: %f\n", x4, y4);

    }
    pixel = ((Uint32*)(image->pixels))[(int)(y4 * image->w + x4)];
    
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    
    return (simd_float3){(float)r, (float)g, (float)b};
}

void findPointsatDists(float x1, float x2, float * x4a, float * x4b, float y1, float y2, float * y4a, float * y4b, float d1, float d2) {
    float AA, BB, XX, YY;
    BB = x1*x1 - x2*x2 + y1*y1 - y2*y2 - d1*d1 + d2*d2;
    XX = x1 - x2;
    YY = y2 - y1;
    
    float A, B, C;
    A = 4*YY*YY + 4*XX*XX;
    B = 4*YY*BB - 8*x2*YY*XX - 8*y2*XX*XX;
    float pc = 4*XX*XX*d2*d2;
    float ppc = 4*XX*XX*y2*y2;
    float pppc = 4*XX*XX*x2*x2;
    float ppppc = 4*x2*XX*BB;
    float pppppc = BB* BB;
    float pn1 = pppppc - ppppc;
    float pn2 = pn1 + pppc;
    float pn3 = pn2 + ppc;
    float pn4 = pn3 - pc;
    C = BB*BB - 4*x2*XX*BB + 4*XX*XX*x2*x2 + 4*XX*XX*y2*y2 - 4*XX*XX*d2*d2;
    
    float xxfa, xxfb, yyfa, yyfb, pre, discriminant;
    pre = fabsf(B*B - (4 * A * C));
    
    discriminant = sqrtf(pre);
    
    yyfa = (-B + discriminant)/(2*A);
    yyfb = (-B - discriminant)/(2*A);
    
    float A2, B2, C2a, C2b, discriminanta, discriminantb;
    A2 = 1;
    B2 = -2 * x1;
    C2a = x2*x2 + ((yyfa - y1) * (yyfa - y1)) - d1*d1;
    C2b = x2*x2 + ((yyfb - y1) * (yyfb - y1)) - d1*d1;
    
    
    discriminanta = fabsf(B2*B2 - (4 * A2 * C2a));
    
    discriminanta = sqrtf(discriminanta);
    
    discriminantb = B2*B2 - (4 * A2 * C2b);
    
    discriminantb = sqrtf(discriminantb);
    
    xxfa = (-B + discriminanta)/(2*A2);
    xxfb = (-B - discriminantb)/(2*A2);
    
    *x4a = xxfa;
    *x4b = xxfb;
    *y4a = yyfa;
    *y4b = yyfb;
}

Object makeTriangle(simd_float3 a, simd_float3 b, simd_float3 c, int material, SDL_Surface* sur) {
    Object object;
    simd_float3 (*inter)(const simd_float3 space, const simd_float3 raydirection,  const struct Object* triangle, const float t) = calcIntersectionTriangle2;
    float (*gt)(const Ray ray, const struct Object* obj) = calcTriangleT;
    if (sur != NULL) {
        printf("HERE");
    }
    object = makeObject(a, b, c, inter, gt, material, 't', sur);
    object.surfaceToImage = triangeImageTransform;
    return object;
}


