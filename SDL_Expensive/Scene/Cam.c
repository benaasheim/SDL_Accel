//
//  Cam.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Cam.h"

simd_float3 position;
simd_quatf rot;

simd_float3 right;
simd_float3 up;
simd_float3 forward;

Uint32 * pixels;
SDL_PixelFormat* SF;

void init_Cam(simd_float3 pos, Uint32 * p, SDL_PixelFormat* S) {
    position = pos;
    rot.vector.y = 1;
    pixels = p;
    SF = S;
    right = simd_act(rot, (simd_float3){1, 0, 0});
    up = simd_act(rot, (simd_float3){0, 1, 0});
    forward = simd_act(rot, (simd_float3){0, 0, 1});
}

void move_cam(simd_float3 dir, float amount) {
    position += dir * amount;
}

void rotate_cam(simd_float3 axis, float theta) {
    rot = simd_normalize(simd_mul(simd_quaternion(theta, axis), rot));
}

void update_cam(const float dt, const unsigned char*  keyboard_keys) {
    //Camera Movement
    if (keyboard_keys[SDL_SCANCODE_W]) {
        move_cam(forward, camera_speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_S]) {
        move_cam(forward, -camera_speed * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_A]) {
        move_cam(right, -camera_speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_D]){
        move_cam(right, camera_speed * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_R]) {
        move_cam(up, camera_speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_F]){
        move_cam(up, -camera_speed * dt);
    }
    
    //Camera Rotation
    if (keyboard_keys[SDL_SCANCODE_DOWN]) {
        rotate_cam(right, camera_sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_UP]) {
        rotate_cam(right, -camera_sensitivity * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_RIGHT]) {
        rotate_cam(up, camera_sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_LEFT]){
        rotate_cam(up, -camera_sensitivity * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_E]) {
        rotate_cam(forward, camera_sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_Q]){
        rotate_cam(forward, -camera_sensitivity * dt);
    }
    
    //update new direction vectors
    right = simd_act(rot, (simd_float3){1, 0, 0});
    up = simd_act(rot, (simd_float3){0, 1, 0});
    forward = simd_act(rot, (simd_float3){0, 0, 1});
}

void cast_rays(void *arguments) {
    // Ray-tracing loop, for each pixel
    int* args = (int *)arguments;
    int start = *args;
    int end = args[1];
    Ray ray;

    ray.origin = position;
    
    simd_float3 col;
    
    int x, y;
    for (y = start; y < end; y++) {
        for (x = 0; x < window_width; x++) {
#if AntiAlias == true
            
            //reset color
            col *= 0;
//            float inc = (1.0f/AntiAliasLevel);
            for (float yy = 0; yy < 1; yy += (1.0f/AntiAliasLevel)) {
                for (float xx = 0; xx < 1; xx += (1.0f/AntiAliasLevel)) {
                    float xxx = x + xx;
                    float yyy = y + yy;
                    ray.direction = simd_normalize(forward + (AA(xxx) * right) + (up * BB(yyy)));
                    col += traceColor2(ray, 0) * 255;
                }
            }
            
            //finish averaging
            col /= AntiAliasLevel*AntiAliasLevel;
            
#else
            ray.direction =
            simd_normalize(forward + (AA(x) * right) + (up * BB(y)));
//            forward;
            col = traceColor2(ray, 0) * 255;
#endif
            pixels[(int)y * window_width + (int)x] = SDL_MapRGB(SF, col.x, col.y, col.z);
        }
    }
}
