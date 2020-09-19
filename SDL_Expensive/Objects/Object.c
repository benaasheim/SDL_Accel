//
//  Object.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Object.h"

Object makeObject(simd_float3 center, simd_float3 p2, simd_float3 p3, simd_float3 (*inter)(const simd_float3 position, const simd_float3 raydirection, const struct Object* triangle, const float t), float (*gt)(const Ray ray, const struct Object* obj), int material, char type, SDL_Surface* sur) {
    Object object;
    object.c = center;
    object.n = p2;
    object.z = p3;
    object.interesect = inter;
    object.gett = gt;
    object.mat = material;
    object.img = sur;
    return object;
}
