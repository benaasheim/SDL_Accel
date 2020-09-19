//
//  Tracing.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Tracing.h"

#define float_max 99999999

float traceShadow(const Ray ray, const Object* thisobj, const Light* light, float L_length) {
//    printf("HERE");
    Hit xFinal;
    Material* m;
    
    //find nearest object intersected by ray
    Object* obj;
    Object* objf = NULL;
    float t;
    
    for (int i = 0; i < numObjects; i++) {
        obj = &objects[i];

        if (thisobj != obj) {
            t = obj->gett(ray, obj);

            if (t > 0.0f && t < L_length) {
                L_length = t;
                objf = obj;
            }
        }
    }
    
    if (objf != NULL) {
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = objf->interesect(xFinal.position, ray.direction, objf, L_length);
    }
    else {
        return 1.0f;
    }
    
    m = &materials[objf->mat];
    
//    simd_float3 tempcol = m->shading_func(ray.direction, xFinal.normal, xFinal.position, light);
    simd_float3 tempcol = _shade(m, &ray, &xFinal, light, objf);
    
    float weight = (tempcol.x + tempcol.y + tempcol.z) / 3.0f;
    
    if (m->reflectivity > 0.0f) {
        weight -= m->reflectivity;
    }
    
    if (m->refractivity > 0.0f) {
        weight *= m->refractivity;
    }
    
    return weight;
}

float traceShadow2(const Ray ray, const void* thisobj, const Light* light, float L_length) {
    Hit xFinal;
    //find nearest object intersected by ray
    float t;
    
    Material* m = NULL;

    simd_float3 dc = {0, 0, 0};
    
    void* ooo = NULL;

    
    Triangle* tri = NULL;
    Triangle* trif = NULL;
    
    for (int i = 0; i < numTriangles; i++) {
        tri = &triangles[i];
        
        if (thisobj != tri) {
            t = calcTriT(ray, tri);
            
            if (t > 0.0f && t < L_length) {
                L_length = t;
                trif = tri;
            }
        }
        
        
    }
    if (trif != NULL) {
        m = &materials[trif->mat];
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionTri2(ray.direction, trif, L_length);
        dc = (trif->img == NULL) ? m->diffuse_color : triImageTransform(xFinal.position, trif);
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
        m = &materials[sphf->mat];
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionSph2(xFinal.position, sphf);
        dc = (sphf->img == NULL) ? m->diffuse_color : sphImageTransform3(xFinal.position, xFinal.normal, sphf);
        ooo = sphf;
    }
    
    Plane* pln = NULL;
    Plane* plnf = NULL;
    
    for (int i = 0; i < numPlanes; i++) {
        pln = &planes[i];
        
        if (thisobj != pln) {
            t = calcPlnT(ray, pln);
            if (t > 0.0f && t < L_length) {
                L_length = t;
                plnf = pln;
            }
        }
    }
    if (plnf != NULL) {
        m = &materials[plnf->mat];
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionPln2(plnf);
        dc = m->diffuse_color;
        ooo = plnf;
    }
    
    if (ooo == NULL) {
        return 1.0f;
    }
        
    simd_float3 tempcol = _shade2(m, &ray, &xFinal, light, dc);
    
    float weight = (tempcol.x + tempcol.y + tempcol.z) / 3.0f;
    
    if (m->reflectivity > 0.0f) {
        weight -= m->reflectivity;
    }
    
    if (m->refractivity > 0.0f) {
        weight *= m->refractivity;
    }
    
    return weight;
}

simd_float3 traceColor(const Ray ray, const int n) {
    Hit xFinal;
    simd_float3 cFinal = {0, 0, 0};
    
    if (n > max_recursion) {
        return cFinal;
    }
    
    //find nearest object intersected by ray
    float L_length = float_max;
    Object* obj = NULL;
    Object* objf = NULL;
    float t;
    
    
    
    #if Intersect_Mode == All
        for (int i = 0; i < numObjects; i++) {
            obj = &objects[i];
            
            t = obj->gett(ray, obj);
            if (t > 0.0f && t < L_length) {
                L_length = t;
                objf = obj;
            }
        }
    
    if (objf == NULL) {
            return cFinal;
        }
        else {
            xFinal.position = ray.direction * L_length + ray.origin;
            xFinal.normal = objf->interesect(xFinal.position, ray.direction, objf, L_length);
        }
    #elif Intersect_Mode == Triangles
    
    simd_float3 plane1n;
        float uDv, wDv, vDv, wDu, uDu, d, s1, t1;
        for (int i = 0; i < numObjects; i++) {
            obj = &objects[i];
            
            simd_float3 u = obj->n - obj->c;
            simd_float3 v = obj->z - obj->c;
            simd_float3 w = obj->c - ray.origin;
            plane1n = simd_normalize(simd_cross(u, v));
            
            d = simd_dot(plane1n, ray.direction);
            
            if (d > 0.0f) {
                plane1n *= -1;
                d *= -1;
            }
            
            t = simd_dot(w, plane1n)/d;
            
            w = ray.direction * t - w;
            uDv = simd_dot(u, v);
            wDv = simd_dot(w, v);
            vDv = simd_dot(v, v);
            wDu = simd_dot(w, u);
            uDu = simd_dot(u, u);
            
            s1 = (uDv*wDv - vDv*wDu)/(uDv*uDv - uDu*vDv);
            t1 = (uDv*wDu - uDu*wDv)/(uDv*uDv - uDu*vDv);
            
            if (s1 < 0.0f || t1 < 0.0f || s1 + t1 >= 1.0f) {
                t = -1.0f;
            }
            if (t > 0.0f && t < L_length) {
                L_length = t;
                objf = obj;
                xFinal.normal = plane1n;
            }
        }
    
    
    #elif Intersect_Mode == 2
    
    #else
    nullErrorCheck(NULL, Invalid_Intersect_Mode);
    #endif
    
    
    if (objf == NULL) {
        return cFinal;
    }
    else {
        xFinal.position = ray.direction * L_length + ray.origin;
        #if Intersect_Mode == All
        xFinal.normal = objf->interesect(xFinal.position, ray.direction, objf, L_length);
        #elif Intersect_Mode == Triangles
//        xFinal.normal = plane1n;
        #elif Intersect_Mode == Circles
        
        #else
        nullErrorCheck(NULL, Invalid_Intersect_Mode);
        #endif
        
        
        
    }
    
    Ray ray_shadow;
    
    Light* l;
    
    ray_shadow.origin = xFinal.position;
    
    Material* m = &materials[objf->mat];
    
    for (int i = 0; i < numLights; i++) {
        l = &lights[i];
        
        cFinal = _shade(m, &ray, &xFinal, l, objf);
        
        if (m->reflectivity != 1.0f) {
            ray_shadow.direction = l->position - xFinal.position;
            float L_length = simd_length(ray_shadow.direction);
            
            ray_shadow.direction = simd_normalize(ray_shadow.direction);
            
            float first = traceShadow(ray_shadow,
                                      objf,
                                      l,
                                      L_length) + m->reflectivity;
            
            first = fminf(1.0f, first);
            
            cFinal *= first;
            
        }
        
    }
    
    if (m->reflectivity > 0.0f) {
        ray_shadow.direction = simd_reflect(ray.direction, xFinal.normal);
        
        ray_shadow.direction = simd_normalize(ray_shadow.direction);
                
        cFinal += traceColor(ray_shadow, n+1) * m->reflectivity;
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
        
        cFinal += traceColor(ray_shadow, n+1) * m->refractivity;
    }
    
    return simd_clamp(cFinal + m->ambient_color, 0, 1);
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
    
    void* ooo = NULL;
    
    Material* m = NULL;

    simd_float3 dc = {0, 0, 0};
    
    Triangle* tri = NULL;
    Triangle* trif = NULL;
    
    for (int i = 0; i < numTriangles; i++) {
        tri = &triangles[i];
        
        t = calcTriT(ray, tri);
        if (t > 0.0f && t < L_length) {
            L_length = t;
            trif = tri;
        }
    }
    if (trif != NULL) {
        m = &materials[trif->mat];
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionTri2(ray.direction, trif, L_length);
        dc = (trif->img == NULL) ? m->diffuse_color : triImageTransform(xFinal.position, trif);
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
        m = &materials[sphf->mat];
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionSph2(xFinal.position, sphf);
        dc = (sphf->img == NULL) ? m->diffuse_color : sphImageTransform3(xFinal.position, xFinal.normal, sphf);
        ooo = sphf;
    }
    
    Plane* pln = NULL;
    Plane* plnf = NULL;
    
    for (int i = 0; i < numPlanes; i++) {
        pln = &planes[i];
        
        t = calcPlnT(ray, pln);
        if (t > 0.0f && t < L_length) {
            L_length = t;
            plnf = pln;
        }
    }
    if (plnf != NULL) {
        m = &materials[plnf->mat];
        xFinal.position = ray.direction * L_length + ray.origin;
        xFinal.normal = calcIntersectionPln2(plnf);
        dc = m->diffuse_color;
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
        
        cFinal = _shade2(m, &ray, &xFinal, l, dc);
        
        if (m->reflectivity != 1.0f) {
            ray_shadow.direction = l->position - xFinal.position;
            float L_length = simd_length(ray_shadow.direction);
            
            ray_shadow.direction = simd_normalize(ray_shadow.direction);
            
            float first = traceShadow2(ray_shadow,
                                       ooo,
                                       l,
                                       L_length) + m->reflectivity;
            
            first = fminf(1.0f, first);
            
            cFinal *= first;
        }
        
    }
    
    if (m->reflectivity > 0.0f) {
        ray_shadow.direction = simd_reflect(ray.direction, xFinal.normal);
        
        ray_shadow.direction = simd_normalize(ray_shadow.direction);
                
        cFinal += traceColor2(ray_shadow, n+1) * m->reflectivity;
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
