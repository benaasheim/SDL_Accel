//
//  Materials.h
//  SDL_ACCEL7
//
//  Created by Ben on 8/8/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Materials_h
#define Materials_h

#include <stdio.h>
#include "Vector.h"
#include "Light.h"
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"

typedef struct Material {
    vec ambient_color;
    float reflectivity;
    float refractivity;
    float ior;
    void (*shading_func)(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a);
} Material;

Material newMaterial(vec ambient_color, vec diffuse_color, vec specular_color, float roughness, float fresnel, float density, float reflectivity, float refractivity, float ior, void (*shading_func)(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a)) {
    Material material;
    material.ambient_color = ambient_color;
    material.reflectivity = reflectivity;
    material.refractivity = refractivity;
    material.ior = ior;
    material.shading_func = shading_func;
    return material;
}

void shade_Diffuse_White(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a) {
    //vec0.01 vecFullRed vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    vec L, H;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    subVec(&l.position, P, &L);
    L_length = mag(&L);
//    divideFloat(&L, L_length, &L);
    divideFloatSelf(L_length, &L);
//    mulfv(-1, V, V);
    mulfvSelf(-1, V);
    addVec(V, &L, &H);
//    normalize(&H, &H);
    normalizeSelf(&H);
    A = l.constant + l.linear * L_length + l.exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, &L), 1.0f);
    NdotV = fminf(dot(N, V), 1.0f);
    NdotH = fminf(dot(N, &H), 1.0f);
    VdotH = fminf(dot(V, &H), 1.0f);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        zeroV(a);
        return;
    }
    
    //Evaluate the geometric term
    lambertian = fminf(1, (fminf(NdotL, NdotV) * 2.0f * NdotH / VdotH));
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    roughness = expf((ndh - 1.0f)/0.140625f/ ndh) / (0.5625 * powf(NdotH, 4));
    
    //Evaluate the fresnel term
    specular = (powf(1.0f-VdotH, 5.0f)/2.0f) + 0.5;
    
    //Combine all
    float Rs = lambertian * roughness * specular * 0.1f / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    float rr = (Rs + 0.9f) * NdotL / A;
    mulfv(rr, &l.color, a);
}

Material Diffuse_White() {
    vec a = {0.01f, 0.01f, 0.01f, 0};
    vec b = {1, 1, 1, 0};
    return newMaterial(a, b, b, 0.375f, 0.5f, 0.9f, 0.0f, 0.0f, 0.0f, shade_Diffuse_White);
}

void shade_Diffuse_Red(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a) {
    //vec0.01 vecFullRed vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    vec L, H;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
        
    subVec(&l.position, P, &L);
    L_length = mag(&L);
    //    divideFloat(&L, L_length, &L);
    divideFloatSelf(L_length, &L);
    //    mulfv(-1, V, V);
    mulfvSelf(-1, V);
    addVec(V, &L, &H);
    //    normalize(&H, &H);
    normalizeSelf(&H);
    A = l.constant + l.linear * L_length + l.exponent * L_length * L_length + epsilon;
        
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, &L), 1.0f);
    NdotV = fminf(dot(N, V), 1.0f);
    NdotH = fminf(dot(N, &H), 1.0f);
    VdotH = fminf(dot(V, &H), 1.0f);
        
        
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        zeroV(a);
        return;
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2 * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    roughness = expf((ndh - 1.0f)/0.140625f/ ndh) / (0.5625f * powf(NdotH, 4));
    
    //Evaluate the fresnel term
    specular = (powf(1-VdotH, 5)/2) + 0.5;
    
    //Combine all
    float Rs = NdotL / A * 0.1f * lambertian * roughness * specular / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    
    vec final_c = {Rs+0.9f, Rs, Rs};
    mulvv(&final_c, &l.color, a);
}

Material Diffuse_Red() {
    vec a = {0.01f, 0.01f, 0.01f, 0};
    vec b = {1, 0, 0, 0};
    vec c = {1, 1, 1, 0};
    return newMaterial(a, b, c, 0.375, 0.5, 0.9, 0.0, 0.0, 0.0, shade_Diffuse_Red);
}

void shade_Diffuse_Blue(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a) {
    //vec0.01 vecFullBlue vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    vec L, H;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    subVec(&l.position, P, &L);
    L_length = mag(&L);
    //    divideFloat(&L, L_length, &L);
    divideFloatSelf(L_length, &L);
    //    mulfv(-1, V, V);
    mulfvSelf(-1, V);
    addVec(V, &L, &H);
    //    normalize(&H, &H);
    normalizeSelf(&H);
    A = l.constant + l.linear * L_length + l.exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, &L), 1.0f);
    NdotV = fminf(dot(N, V), 1.0f);
    NdotH = fminf(dot(N, &H), 1.0f);
    VdotH = fminf(dot(V, &H), 1.0f);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        zeroV(a);
        return;
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2 * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    roughness = expf((ndh - 1.0f)/0.140625f/ ndh) / (0.5625f * powf(NdotH, 4));
    
    //Evaluate the fresnel term
    specular = (powf(1-VdotH, 5)/2) + 0.5f;
    
    //Combine all
    float Rs = NdotL / A * lambertian * roughness * specular * 0.1f / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    vec final_c = {Rs, Rs, Rs+0.9f, 0};
    mulvv(&final_c, &l.color, a);
}

Material Diffuse_Blue() {
    vec a = {0.01f, 0.01f, 0.01f, 0};
    vec b = {0, 0, 1, 0};
    vec c = {1, 1, 1, 0};
    return newMaterial(a, b, c, 0.375, 0.5, 0.9, 0.0, 0.0, 0.0, shade_Diffuse_Blue);
}

void shade_Reflective_Metal(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a) {
    //vec0.01 vec0.0 vec1.0
    //rgh0.1 frs1.0 dns0.5 rfl1.0 rfr0.0 ior0.0
    vec L, H;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    subVec(&l.position, P, &L);
    L_length = mag(&L);
    //    divideFloat(&L, L_length, &L);
    divideFloatSelf(L_length, &L);
    //    mulfv(-1, V, V);
    mulfvSelf(-1, V);
    addVec(V, &L, &H);
    //    normalize(&H, &H);
    normalizeSelf(&H);
    A = l.constant + l.linear * L_length + l.exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, &L), 1.0f);
    NdotV = fminf(dot(N, V), 1.0f);
    NdotH = fminf(dot(N, &H), 1.0f);
    VdotH = fminf(dot(V, &H), 1.0f);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        zeroV(a);
        return;
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2 * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    float roughness_b = ndh - 1;
    float roughness_c = 0.01 * ndh;
    roughness = (float) expf(roughness_b/roughness_c) / (0.04 * powf(NdotH, 4));
    
    //Combine all
    float Rs = lambertian * roughness * 1 / fmaxf(NdotL * NdotV, epsilon) / 2 * NdotL / A;

    mulfv(Rs, &l.color, a);
}

Material Reflective_Metal() {
    vec a = {0.01f, 0.01f, 0.01f, 0};
    vec b = {0, 0, 0, 0};
    vec c = {1, 1, 1, 0};
    return newMaterial(a, b, c, 0.1, 1.0, 0.5, 1.0, 0.0, 0.0, shade_Reflective_Metal);
}

void shade_Refractive_Glass(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a) {
    //vec0.01 vec0.0 vec1.0
    //rgh0.1 frs1.0 dns0.5 rfl0.0 rfr1.0 ior2.0
    vec L, H;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    subVec(&l.position, P, &L);
    L_length = mag(&L);
    //    divideFloat(&L, L_length, &L);
    divideFloatSelf(L_length, &L);
    //    mulfv(-1, V, V);
    mulfvSelf(-1, V);
    addVec(V, &L, &H);
    //    normalize(&H, &H);
    normalizeSelf(&H);
    A = l.constant + l.linear * L_length + l.exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, &L), 1.0f);
    NdotV = fminf(dot(N, V), 1.0f);
    NdotH = fminf(dot(N, &H), 1.0f);
    VdotH = fminf(dot(V, &H), 1.0f);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        zeroV(a);
        return;
    }   return;
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2.0f * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    float roughness_b = ndh - 1.0f;
    float roughness_c = 0.01f * ndh;
    roughness = expf(roughness_b/roughness_c) / (0.04f * powf(NdotH, 4));
    
//    //Combine all
//    float Rs = lambertian * roughness / fmaxf(NdotL * NdotV, 1e-3f) / 2 * NdotL / A;
//
//    mulfv(Rs, &l.color, a);
    //Combine all
    float Rs = lambertian * roughness / fmaxf(NdotL * NdotV, 1e-3f);
    vec vt;
    mulfv(NdotL / A, &l.color, &vt);
    float rr = Rs / 2;
    
    mulfv(rr, &vt, a);
}

Material Refractive_Glass() {
    vec a = {0.01f, 0.01f, 0.01f, 0};
    vec b = {0, 0, 0, 0};
    vec c = {1, 1, 1, 0};
    return newMaterial(a, b, c, 0.1f, 1.0f, 0.75f, 0.0f, 1.0f, 1.12f, shade_Refractive_Glass);
}

void shade_Diffuse_Sky_Blue(float * restrict V, float * restrict N, float * restrict P, Light l, float * restrict a) {
    //vec0.01 vecFullBlue vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    vec L, H;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    subVec(&l.position, P, &L);
    L_length = mag(&L);
    //    divideFloat(&L, L_length, &L);
    divideFloatSelf(L_length, &L);
//    divideFloatSelf(L_length, &L);
    //    mulfv(-1, V, V);
    mulfvSelf(-1, V);
    addVec(V, &L, &H);
    //    normalize(&H, &H);
    normalizeSelf(&H);
    A = l.constant + l.linear * L_length + l.exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, &L), 1.0f);
    NdotV = fminf(dot(N, V), 1.0f);
    NdotH = fminf(dot(N, &H), 1.0f);
    VdotH = fminf(dot(V, &H), 1.0f);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        zeroV(a);
        return;
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2 * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    float roughness_b = ndh - 1;
    float roughness_c = 0.1 * ndh;
    roughness = expf(roughness_b/roughness_c) / (0.4 * powf(NdotH, 4));
    
    //Evaluate the fresnel term
    specular = (powf(1-VdotH, 5)/2) + 0.5;
    
    //Combine all
    float Rs =
//    NdotL / A *
    lambertian *
//    0.1 *
    roughness * specular / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
//    vec final_a = scalevec(NdotL / A, l.color);
//    vec final_b = addvec(newvec(0.2f, 0.2f, 0.9f), scalevec(Rs, newvec(0.1, 0.1, 0.1)));
//    vec v1 = {Rs + 0.2f, Rs + 0.2f, Rs + 0.9f, 0};
    
    vec final_b = {Rs+0.2, Rs+0.2, Rs+0.9};
    
    mulfv(NdotL / A, &l.color, a);
    mulvv(&final_b, a, a);
}

Material Diffuse_Sky_Blue() {
    vec a = {0.2f, 0.25f, 0.75f, 0};
    vec b = {0.2f, 0.2f, 0.9f, 0};
    vec c = {1, 1, 1, 0};
    return newMaterial(a, b, c, 0.375, 0.5, 0.9, 0.0, 0.0, 0.0, shade_Diffuse_Sky_Blue);
}

#endif /* Materials_h */
