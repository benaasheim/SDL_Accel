//
//  Material.c
//  SDL_Expensive
//
//  Created by Ben on 9/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Material.h"
#define PHONG 1
#define COOKTORRANCE 0

simd_float3 _shade2(Material* mat, const Ray* ray, Hit* hit, const Light* l, simd_float3 (*surfaceToImage)(Hit* hit, void* obj), const void* obj, const SDL_Surface* surf) {

    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1.0f;
    simd_float3 C = {0, 0, 0}, L, V, H, N;
    simd_float3 dc;
    N = hit->normal;

    if (l->type == DIRECTIONAL) {
        L = simd_normalize(-l->direction);
        V = -ray->direction;
        H = simd_normalize(V + L);
    }
    else if (l->type == POINT) {
        L = l->position - hit->position;
        L_length = simd_length(L);
        L = simd_normalize(L);
        V = -ray->direction;
        H = simd_normalize(V + L);
        A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    }
    
    //Calculate the dot products required for shading
    NdotL = fminf(simd_dot(N, L), 1.0f);
    NdotV = fminf(simd_dot(N, V), 1.0f);
    NdotH = fminf(simd_dot(N, H), 1.0f);
    VdotH = fminf(simd_dot(V, H), 1.0f);
    
    if (mat->type == PHONG) {
        // Calculate the lambertian term
        lambertian = fminf(NdotL, 1.0f);
        
        // Calculate the specular term
        if (mat->shininess > 0.0f) {
            specular = powf(NdotH, mat->shininess);
        }
        else {
            specular = 0.0f;
        }
        
        //Add all the terms to C
        dc = (surf == NULL) ? mat->diffuse_color : surfaceToImage(hit, obj);
        C = C + (l->color * dc * lambertian * l->intensity);
        C = C + (mat->specular_color * specular * l->intensity);
    }
    else if (mat->type == COOKTORRANCE) {
        //Return NULL color if the surface normal and light direction are facing opposite directions
        if (NdotL < epsilon) {
            return C;
        }
        
        //Get the surface properties
        float R = mat->roughness;
        float F = mat->fresnel;
        float K = mat->density;
        
        // Evaluate the geometric term
        float geo_numerator = 2.0f * NdotH;
        float geo_a = (geo_numerator * NdotV) / VdotH;
        float geo_b = (geo_numerator * NdotL) / VdotH;
        float f1 = fminf(geo_a, geo_b);
        lambertian = fminf(1.0f, f1);
        
        // Evaluate the roughness term
        float roughness_a = 1.0f / (4.0f * R * R * (float) powf(NdotH, 4));
        float roughness_b = NdotH * NdotH - 1.0f;
        float roughness_c = R * R * NdotH * NdotH;
        roughness = roughness_a * expf(roughness_b/roughness_c);
        
        
        // Evaluate the fresnel term
        specular = powf(1.0f - VdotH, 5);
        specular *= 1.0f - F;
        specular += F;
        
        // Put all the terms together
        float Rs_numerator = lambertian * roughness * specular;
        float Rs_denominator = fmaxf(NdotV * NdotL, epsilon);
        float Rs = Rs_numerator / Rs_denominator;
        
        //Add all the terms to C
        simd_float3 final_a = NdotL * l->color * l->intensity;
        

        
        dc = (surf == NULL) ? mat->diffuse_color : surfaceToImage(hit, obj);
        
        
        simd_float3 final_b = dc * K + mat->specular_color * Rs * (1.0f - K);
        C = final_a * final_b;
    }
    else {
        return C;
    }
    
    return C/A;
}

simd_float3 _shade3(Material* mat, const Ray* ray, Hit* hit, const Light* l, simd_float3 (*surfaceToImage)(Hit* hit, void* obj), const void* obj, const SDL_Surface* surf) {

    float NdotL, NdotV, NdotH, VdotH, lambertian, specular, roughness, L_length, A = 1.0f;
    simd_float3 C = {0, 0, 0}, L, V, H, N;
    simd_float3 dc;
    N = hit->normal;

    if (l->type == DIRECTIONAL) {
        L = simd_normalize(-l->direction);
        V = -ray->direction;
        H = simd_normalize(V + L);
    }
    else if (l->type == POINT) {
        L = l->position - hit->position;
        L_length = simd_length(L);
        L = simd_normalize(L);
        V = -ray->direction;
        H = simd_normalize(V + L);
        A = l->constant + l->linear * L_length + l->exponent * L_length * L_length + epsilon;
    }
    
    //Calculate the dot products required for shading
    NdotL = fminf(simd_dot(N, L), 1.0f);
    NdotV = fminf(simd_dot(N, V), 1.0f);
    NdotH = fminf(simd_dot(N, H), 1.0f);
    VdotH = fminf(simd_dot(V, H), 1.0f);
    
    if (mat->type == PHONG) {
        // Calculate the lambertian term
        lambertian = fminf(NdotL, 1.0f);
        // Calculate the specular term
        if (mat->shininess > 0.0f) {
            specular = powf(NdotH, mat->shininess);
        }
        else {
            specular = 0.0f;
        }
        
        //Add all the terms to C
        dc = (surf == NULL) ? mat->diffuse_color : surfaceToImage(hit, obj);
        C = C + (l->color * dc * lambertian * l->intensity);
        C = C + (mat->specular_color * specular * l->intensity);
    }
    else if (mat->type == COOKTORRANCE) {
        //Return NULL color if the surface normal and light direction are facing opposite directions
        if (NdotL < epsilon) {
            return C;
        }
        
        //Get the surface properties
        float R = mat->roughness;
        float F = mat->fresnel;
        float K = mat->density;
        
        // Evaluate the geometric term
        float geo_numerator = 2.0f * NdotH;
        float geo_a = (geo_numerator * NdotV) / VdotH;
        float geo_b = (geo_numerator * NdotL) / VdotH;
        float f1 = fminf(geo_a, geo_b);
        lambertian = fminf(1.0f, f1);
        
        // Evaluate the roughness term
        float roughness_a = 1.0f / (4.0f * R * R * (float) powf(NdotH, 4));
        float roughness_b = NdotH * NdotH - 1.0f;
        float roughness_c = R * R * NdotH * NdotH;
        roughness = roughness_a * expf(roughness_b/roughness_c);
        
        
        // Evaluate the fresnel term
        specular = powf(1.0f - VdotH, 5);
        specular *= 1.0f - F;
        specular += F;
        
        // Put all the terms together
        float Rs_numerator = lambertian * roughness * specular;
        float Rs_denominator = fmaxf(NdotV * NdotL, epsilon);
        float Rs = Rs_numerator / Rs_denominator;
        
        //Add all the terms to C
        simd_float3 final_a = NdotL * l->color * l->intensity;
        

        
        dc = (surf == NULL) ? mat->diffuse_color : surfaceToImage(hit, obj);
        
        
        simd_float3 final_b = dc * K + mat->specular_color * Rs * (1.0f - K);
        C = final_a * final_b;
    }
    else {
        return C;
    }
    
    return C/A;
}
