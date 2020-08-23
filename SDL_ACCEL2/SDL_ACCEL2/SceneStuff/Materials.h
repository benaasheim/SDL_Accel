//
//  Materials.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Materials_h
#define Materials_h
#include "Vector.h"
#include "Light.h"
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"

typedef struct Material {
    Vector ambient_color;
    Vector diffuse_color;
    float reflectivity;
    float refractivity;
    float ior;
    Vector (*shading_func)(Ray* ray, Hit* hit, Light* l);
} Material;

Material newMaterial(Vector ambient_color, Vector diffuse_color, Vector specular_color, float roughness, float fresnel, float density, float reflectivity, float refractivity, float ior, Vector (*shading_func)(Ray* ray, Hit* hit, Light* l)) {
    Material material;
    material.ambient_color = ambient_color;
    material.diffuse_color = diffuse_color;
    material.reflectivity = reflectivity;
    material.refractivity = refractivity;
    material.ior = ior;
    material.shading_func = shading_func;
    return material;
}

Vector shade_Diffuse_White(Ray* ray, Hit* hit, Light* l) {
    //vec0.01 vecFullRed vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    Vector L, V, H;
    Vector N = hit->normal;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    L = minusVector(l->position, hit->position);
    L_length = vLength(L);
    L = divideFloat(L_length, L);
    V = scaleVector(-1, ray->direction);
    H = normalize(addVector(V, L));
    A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, L), 1);
    NdotV = fminf(dot(N, V), 1);
    NdotH = fminf(dot(N, H), 1);
    VdotH = fminf(dot(V, H), 1);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        return newVector(0, 0, 0);
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2 * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    float roughness_b = ndh - 1;
    float roughness_c = 0.140625 * ndh;
    roughness = expf(roughness_b/roughness_c) / (0.5625 * powf(NdotH, 4));
    
    //Evaluate the fresnel term
    specular = (powf(1-VdotH, 5)/2) + 0.5;
    
    //Combine all
    float Rs = lambertian * roughness * specular / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    Vector final_a = scaleVector(NdotL / A, l->color);
    float rr = Rs/10 + 0.9;
    Vector final_b = newVector(rr, rr, rr);
    return scaleV(final_a, final_b);
}

Material Diffuse_White() {
    Vector a = newVector(0.01f, 0.01f, 0.01f);
    Vector b = newVector(1.0f, 1.0f, 1.0f);
    return newMaterial(a, b, b, 0.375f, 0.5f, 0.9f, 0.0f, 0.0f, 0.0f, shade_Diffuse_White);
}

Vector shade_Diffuse_Red(Ray* ray, Hit* hit, Light* l) {
    //vec0.01 vecFullRed vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    Vector L, V, H;
    Vector N = hit->normal;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    L = minusVector(l->position, hit->position);
    L_length = vLength(L);
    L = divideFloat(L_length, L);
    V = scaleVector(-1, ray->direction);
    H = normalize(addVector(V, L));
    A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, L), 1);
    NdotV = fminf(dot(N, V), 1);
    NdotH = fminf(dot(N, H), 1);
    VdotH = fminf(dot(V, H), 1);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        return newVector(0, 0, 0);
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2 * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    float roughness_b = ndh - 1;
    float roughness_c = 0.140625 * ndh;
    roughness = expf(roughness_b/roughness_c) / (0.5625 * powf(NdotH, 4));
    
    //Evaluate the fresnel term
    specular = (powf(1-VdotH, 5)/2) + 0.5;
    
    //Combine all
    float Rs = lambertian * roughness * specular / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    Vector final_a = scaleVector(NdotL  / A, l->color);
    float rr = Rs/10;
    Vector final_b = newVector(rr+0.9f, rr, rr);
    
    return scaleV(final_a, final_b);
}

Material Diffuse_Red() {
    Vector a = newVector(0.01, 0.01, 0.01);
    Vector b = newVector(1, 0, 0);
    Vector c = newVector(1, 1, 1);
    return newMaterial(a, b, c, 0.375, 0.5, 0.9, 0.0, 0.0, 0.0, shade_Diffuse_Red);
}

Vector shade_Diffuse_Blue(Ray* ray, Hit* hit, Light* l) {
    //vec0.01 vecFullBlue vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    Vector L, V, H;
    Vector N = hit->normal;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    L = minusVector(l->position, hit->position);
    L_length = vLength(L);
    L = divideFloat(L_length, L);
    V = scaleVector(-1, ray->direction);
    H = normalize(addVector(V, L));
    A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, L), 1);
    NdotV = fminf(dot(N, V), 1);
    NdotH = fminf(dot(N, H), 1);
    VdotH = fminf(dot(V, H), 1);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        return newVector(0, 0, 0);
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2 * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    float roughness_b = ndh - 1;
    float roughness_c = 0.140625 * ndh;
    roughness = expf(roughness_b/roughness_c) / (0.5625 * powf(NdotH, 4));
    
    //Evaluate the fresnel term
    specular = (powf(1-VdotH, 5)/2) + 0.5;
    
    //Combine all
    float Rs = lambertian * roughness * specular / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    Vector final_a = scaleVector(NdotL / A, l->color);
    Vector final_b = addVector(newVector(0, 0, 0.9), scaleVector(Rs, newVector(0.1, 0.1, 0.1)));
    
    return scaleV(final_a, final_b);
}

Material Diffuse_Blue() {
    Vector a = newVector(0.01, 0.01, 0.01);
    Vector b = newVector(0, 0, 1);
    Vector c = newVector(1, 1, 1);
    return newMaterial(a, b, c, 0.375, 0.5, 0.9, 0.0, 0.0, 0.0, shade_Diffuse_Blue);
}

Vector shade_Reflective_Metal(Ray* ray, Hit* hit, Light* l) {
    //vec0.01 vec0.0 vec1.0
    //rgh0.1 frs1.0 dns0.5 rfl1.0 rfr0.0 ior0.0
    Vector L, V, H;
    Vector N = hit->normal;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    L = minusVector(l->position, hit->position);
    L_length = vLength(L);
    L = divideFloat(L_length, L);
    V = scaleVector(-1, ray->direction);
    H = normalize(addVector(V, L));
    A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, L), 1);
    NdotV = fminf(dot(N, V), 1);
    NdotH = fminf(dot(N, H), 1);
    VdotH = fminf(dot(V, H), 1);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        return newVector(0, 0, 0);
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
    float Rs = lambertian * roughness * 1 / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    Vector final_a = scaleVector(NdotL / A, l->color);
    float rr = Rs/2;

    return scaleVector(rr, final_a);
}

Material Reflective_Metal() {
    Vector a = newVector(0.01, 0.01, 0.01);
    Vector b = newVector(0, 0, 0);
    Vector c = newVector(1, 1, 1);
    return newMaterial(a, b, c, 0.1, 1.0, 0.5, 1.0, 0.0, 0.0, shade_Reflective_Metal);
}

Vector shade_Refractive_Glass(Ray* ray, Hit* hit, Light* l) {
    //vec0.01 vec0.0 vec1.0
    //rgh0.1 frs1.0 dns0.5 rfl0.0 rfr1.0 ior2.0
    
    Vector L, V, H;
    Vector N = hit->normal;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    L = minusVector(l->position, hit->position);
    L_length = vLength(L);
    L = divideFloat(L_length, L);
    V = scaleVector(-1, ray->direction);
    H = normalize(addVector(V, L));
    A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, L), 1);
    NdotV = fminf(dot(N, V), 1);
    NdotH = fminf(dot(N, H), 1);
    VdotH = fminf(dot(V, H), 1);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        return newVector(0, 0, 0);
    }
    
    //Evaluate the geometric term
    float geo_numerator = fminf(NdotL, NdotV) * 2.0f * NdotH / VdotH;
    lambertian = fminf(1, geo_numerator);
    
    //Evaluate the roughness term
    float ndh = NdotH * NdotH;
    float roughness_b = ndh - 1.0f;
    float roughness_c = 0.01f * ndh;
    roughness = expf(roughness_b/roughness_c) / (0.04f * powf(NdotH, 4));
    
    //Combine all
    float Rs = lambertian * roughness / fmaxf(NdotL * NdotV, 1e-3f);
    
    //Add all terms to C
    Vector final_a = scaleVector(NdotL / A, l->color);
    float rr = Rs /2;
    return scaleVector(rr, final_a);
}

Material Refractive_Glass() {
    Vector a = newVector(0.01f, 0.01f, 0.01f);
    Vector b = newVector(0, 0, 0);
    Vector c = newVector(1, 1, 1);
    return newMaterial(a, b, c, 0.1f, 1.0f, 0.75f, 0.0f, 1.0f, 2.0f, shade_Refractive_Glass);
}

Vector shade_Diffuse_Sky_Blue(Ray* ray, Hit* hit, Light* l) {
    //vec0.01 vecFullBlue vec1.0
    //rgh0.375 frs0.5 dns0.9 rfl0.0 rfr0.0 ior0.0
    Vector L, V, H;
    Vector N = hit->normal;
    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1;
    
    L = minusVector(l->position, hit->position);
    L_length = vLength(L);
    L = divideFloat(L_length, L);
    V = scaleVector(-1, ray->direction);
    H = normalize(addVector(V, L));
    A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    
    //Calculate the dot products required for shading
    NdotL = fminf(dot(N, L), 1);
    NdotV = fminf(dot(N, V), 1);
    NdotH = fminf(dot(N, H), 1);
    VdotH = fminf(dot(V, H), 1);
    
    
    // Return NULL color if the surface normal and light direction are facing opposite directions
    if (NdotL < epsilon) {
        return newVector(0, 0, 0);
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
    float Rs = lambertian * roughness * specular / fmaxf(NdotL * NdotV, epsilon);
    
    //Add all terms to C
    Vector final_a = scaleVector(NdotL / A, l->color);
    Vector final_b = addVector(newVector(0.2f, 0.2f, 0.9f), scaleVector(Rs, newVector(0.1, 0.1, 0.1)));
    
    return scaleV(final_a, final_b);
}

Material Diffuse_Sky_Blue() {
    Vector a = newVector(0.2f, 0.25f, 0.75f);
    Vector b = newVector(0.2f, 0.2f, 0.9f);
    Vector c = newVector(1, 1, 1);
    return newMaterial(a, b, c, 0.375, 0.5, 0.9, 0.0, 0.0, 0.0, shade_Diffuse_Sky_Blue);
}

#endif /* Materials_h */
