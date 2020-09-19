//
//  Cam.h
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Cam_h
#define Cam_h

#include <stdio.h>
#include <simd/simd.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#include "Settings.h"
#include "Ray.h"
#include "Utilities.h"
#include "Tracing.h"

void init_Cam(simd_float3 position, Uint32 * p, SDL_PixelFormat* S);

void move_cam(simd_float3 dir, float amount);

void rotate_cam(simd_float3 axis, float theta);

void update_cam(const float dt, const unsigned char*  keyboard_keys);

void cast_rays(void *arguments);

#endif /* Cam_h */
