//
//  Materials.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Materials_h
#define Materials_h

#include <stdio.h>
#include "Light.h"
#include "Hit.h"
#include "Settings.h"
#include "Ray.h"

struct __attribute__ ((__packed__)) Material {
    simd_float3 ambient_color;
    simd_float3 diffuse_color;
    simd_float3 specular_color;
    simd_float3 (*shading_func)(simd_float3 raydir,
                                simd_float3 N,
                                simd_float3 P,
                                Light* l);
    float roughness;
    float fresnel;
    float density;
    float reflectivity;
    float refractivity;
    float ior;
};

typedef struct Material Material;

Material newMaterial(simd_float3 ambient_color,
                     simd_float3 diffuse_color,
                     simd_float3 specular_color,
                     float roughness,
                     float fresnel,
                     float density,
                     float reflectivity,
                     float refractivity,
                     float ior,
                     simd_float3 (*shading_func)(simd_float3 raydir,
                                                 simd_float3 N,
                                                 simd_float3 P,
                                                 Light* l)
                     );

simd_float3 shade_Diffuse_White_Fast(simd_float3 raydir,
                                     simd_float3 N,
                                     simd_float3 P,
                                     Light* l);
Material Diffuse_White(void);

simd_float3 shade_Diffuse_Red_Fast(simd_float3 raydir,
                                   simd_float3 N,
                                   simd_float3 P,
                                   Light* l);

Material Diffuse_Red(void);

simd_float3 shade_Diffuse_Blue_Fast(simd_float3 raydir,
                                    simd_float3 N,
                                    simd_float3 P,
                                    Light* l);

Material Diffuse_Blue(void);

simd_float3 shade_Reflective_Metal_Fast(simd_float3 raydir,
                                        simd_float3 N,
                                        simd_float3 P,
                                        Light* l);
Material Reflective_Metal(void);

simd_float3 shade_Refractive_Glass_Fast(simd_float3 raydir,
                                        simd_float3 N,
                                        simd_float3 P,
                                        Light* l);

Material Refractive_Glass(void);

simd_float3 shade_Diffuse_Sky_Blue_Fast(simd_float3 raydir,
                                        simd_float3 N,
                                        simd_float3 P,
                                        Light* l);

Material Diffuse_Sky_Blue(void);

#endif /* Materials_h */
