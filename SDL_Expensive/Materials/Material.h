//
//  Material.h
//  SDL_Expensive
//
//  Created by Ben on 9/9/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Material_h
#define Material_h

#include <stdio.h>
#include "Object.h"
#include "Light.h"
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"

struct __attribute__ ((__packed__)) Material {
    simd_float3 ambient_color;
    simd_float3 diffuse_color;
    simd_float3 specular_color;
    float roughness;
    float fresnel;
    float density;
    float reflectivity;
    float refractivity;
    float ior;
    float shininess;
    int type;
};

typedef struct Material Material;

simd_float3 _shade(Material* mat, Ray* ray, Hit* hit, Light* l, Object* obj);

simd_float3 _shade2(Material* mat, const Ray* ray, Hit* hit, const Light* l, simd_float3 dc);

Material Diffuse_White(void);

Material Diffuse_Red(void);

Material Diffuse_Blue(void);

Material Reflective_Metal(void);

Material Refractive_Glass(void);

Material Diffuse_Sky_Blue(void);

#endif /* Material_h */
