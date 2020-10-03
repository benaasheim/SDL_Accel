//
//  Tracing.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Tracing.h"

#define float_max 99999999

float traceShadow2(const Ray ray, const void* thisobj, const Light* light, float L_length) {
    Hit xFinal;
    //find nearest object intersected by ray
    float t;
    
    Material* m = NULL;
    
    void* ooo = NULL;
    
    simd_float3 normal;


    simd_float3 (*surfaceToImage)(Hit* hit, void* obj);
    SDL_Surface* surf = NULL;
    
    Triangle* tri = NULL;
    Triangle* trif = NULL;
    
    
    for (int i = 0; i < numTriangles; i++) {
        tri = &triangles[i];
        
        if (thisobj != tri) {
            t = calcTriT(ray, tri, &normal);
            
            if (t > 0.0f && t < L_length) {
                L_length = t;
                trif = tri;
                xFinal.normal = normal;
            }
        }
        
        
    }
    
    if (trif != NULL) {
        m = trif->mat;
        xFinal.position = ray.direction * L_length + ray.origin;
        surfaceToImage = triImageTransform2;
        surf = trif->img;
        ooo = trif;
    }
    
    Sphere* sph = NULL;
    Sphere* sphf = NULL;
    
    for (int i = 0; i < numSpheres; i++) {
        sph = &spheres[i];
        
        if (thisobj != sph) {
            t = calcSphT(ray, sph);
            if (t > 0.0f && t < L_length) {
                L_length = t;
                sphf = sph;
            }
        }
    }
    if (sphf != NULL) {
        m = sphf->mat;
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionSph2(xFinal.position, sphf);
        surfaceToImage = sphImageTransform3;
        surf = sphf->img;
        ooo = sphf;
    }
    
    Plane* pln = NULL;
    Plane* plnf = NULL;
    for (int i = 0; i < numPlanes; i++) {
        pln = &planes[i];
        
        if (thisobj != pln) {
            t = calcPlnT(ray, pln, &normal);
            if (t > 0.0f && t < L_length) {
                L_length = t;
                plnf = pln;
                xFinal.normal = normal;
            }
        }
    }
    if (plnf != NULL) {
        m = plnf->mat;
        xFinal.position = ray.direction * L_length + ray.origin;
        surf = NULL;
        ooo = plnf;
    }
    
    if (ooo == NULL) {
        return 1.0f;
    }
        
    simd_float3 tempcol = _shade2(m, &ray, &xFinal, light, surfaceToImage, ooo, surf);
    
    float weight = (tempcol.x + tempcol.y + tempcol.z) / 3.0f;
    
    if (m->reflectivity > 0.0f) {
        weight -= m->reflectivity;
    }
    
    if (m->refractivity > 0.0f) {
        weight *= m->refractivity;
    }
    
    return weight;
}

simd_float3 traceColor2(const Ray ray, const int n) {
    Hit xFinal;
    simd_float3 cFinal = {0, 0, 0};
    
    if (n > max_recursion) {
        return cFinal;
    }
    
    //find nearest object intersected by ray
    float L_length = float_max;
    float t;
    
    simd_float3 normal;

    void* ooo = NULL;
    simd_float3 (*surfaceToImage)(Hit* hit, void* obj);
    SDL_Surface* surf = NULL;
    Material* m = NULL;
    
    Triangle* tri = NULL;
    Triangle* trif = NULL;
    
    for (int i = 0; i < numTriangles; i++) {
        tri = &triangles[i];
        
        t = calcTriT(ray, tri, &normal);
        if (t > 0.0f && t < L_length) {
            L_length = t;
            trif = tri;
            xFinal.normal = normal;
        }
    }
    if (trif != NULL) {
        m = trif->mat;
        xFinal.position = ray.direction * L_length + ray.origin;
//        xFinal.normal = calcIntersectionTri2(ray.direction, trif, L_length);
        surfaceToImage = triImageTransform2;
        surf = trif->img;
        ooo = trif;
    }
    
    Sphere* sph = NULL;
    Sphere* sphf = NULL;
    
    for (int i = 0; i < numSpheres; i++) {
        sph = &spheres[i];
        
        t = calcSphT(ray, sph);
        if (t > 0.0f && t < L_length) {
            L_length = t;
            sphf = sph;
        }
    }
    if (sphf != NULL) {
        m = sphf->mat;
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionSph2(xFinal.position, sphf);
        surfaceToImage = sphImageTransform3;
        surf = sphf->img;
        ooo = sphf;
    }
    
    Plane* pln = NULL;
    Plane* plnf = NULL;

    for (int i = 0; i < numPlanes; i++) {
        pln = &planes[i];
        
        t = calcPlnT(ray, pln, &normal);
        if (t > 0.0f && t < L_length) {
            L_length = t;
            plnf = pln;
            xFinal.normal = normal;
        }
    }
    if (plnf != NULL) {
        m = plnf->mat;
        xFinal.position = ray.direction * L_length + ray.origin;
//        xFinal.normal = calcIntersectionPln2(plnf);
        surf = NULL;
        ooo = plnf;
    }
    
    if (ooo == NULL) {
        return cFinal;
    }
    
    Ray ray_shadow;
    
    Light* l;
    
    ray_shadow.origin = xFinal.position;
        
    for (int i = 0; i < numLights; i++) {
        l = &lights[i];
        
        cFinal = _shade3(m, &ray, &xFinal, l, surfaceToImage, ooo, surf);
        
        if (m->reflectivity != 1.0f) {
            
        ray_shadow.direction = l->position - xFinal.position;
        float L_length = simd_length(ray_shadow.direction);

        
        
        ray_shadow.direction = simd_normalize(ray_shadow.direction);

        
        float first = traceShadow2(ray_shadow,
                                   ooo,
                                   l,
                                   L_length)
        + m->reflectivity
        ;
        
        first = fminf(1.0f, first);
        
        cFinal *= first;
        }
        
    }
    
    if (m->reflectivity > 0.0f) {
        ray_shadow.direction = simd_reflect(ray.direction, xFinal.normal);

        ray_shadow.direction = simd_normalize(ray_shadow.direction);

        if (m->frosting > 1) {

            float range = 50 * epsilon;

            ray_shadow.direction.x -= range/2;
            ray_shadow.direction.y -= range/2;
            ray_shadow.direction.z -= range/2;

            simd_float3 frost_color = {0, 0, 0};

            for (int i = 0; i < m->frosting; i++) {
                ray_shadow.direction.x += range/m->frosting;
                for (int j = 0; j < m->frosting; j++) {
                    ray_shadow.direction.y += range/m->frosting;
                    for (int k = 0; k < m->frosting; k++) {
                        ray_shadow.direction.z += range/m->frosting;
                        frost_color += traceColor2(ray_shadow, n+1) * m->reflectivity;
                    }
                }
            }
            frost_color /= (m->frosting * m->frosting * m->frosting);
            cFinal += frost_color;
        }
        else {
            cFinal += traceColor2(ray_shadow, n+1) * m->reflectivity;
        }
    }
    
    if (m->refractivity > 0.0f) {
        float NdotI, cos_t, ior;
        ior = m->ior;
        NdotI = simd_dot(ray.direction, xFinal.normal);
        
        if (NdotI > 0) {
            xFinal.normal *= -1;
        }
        else {
            ior = 1/ior;
            NdotI = -NdotI;
        }
        
        cos_t = ior * ior * (1.0f - NdotI * NdotI);
        cos_t = sqrtf(1.0f - cos_t);
        
        if (cos_t < 1.0f) {
            ray_shadow.direction = (ior*NdotI-cos_t) * xFinal.normal + ior * ray.direction;
        }
        else {
            ray_shadow.direction = simd_reflect(ray.direction, xFinal.normal);
        }
        
        ray_shadow.direction = simd_normalize(ray_shadow.direction);
        
        cFinal += traceColor2(ray_shadow, n+1) * m->refractivity;
    }
    
    return simd_clamp(cFinal + m->ambient_color, 0, 1);
}
