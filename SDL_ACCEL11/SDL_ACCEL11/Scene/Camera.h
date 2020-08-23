//
//  Camera.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <stdio.h>
#include <SDL2/SDL.h>

#include "Ray.h"
#include "Settings.h"


#define AA(i) (((i%window_width) - window_width * 0.5)/window_width*aspect_ratio)
#define BB(i) ((window_height * 0.5 - (i/window_width))/window_height)

struct __attribute__ ((__packed__)) Camera {
    simd_float3 position;
    simd_quatf rot;
    float speed;
    float sensitivity;
};

typedef struct Camera Camera;

Camera newCamera(simd_float3 position, float sp, float se);

void move_camera(Camera* c, simd_float3 dir, float amount);

void rotate_camera(Camera* c, simd_float3 axis, float theta);


simd_float3 camera_Forward(Camera* c);

simd_float3 camera_Right(Camera* c);

simd_float3 camera_Up(Camera* c);

Ray calcCameraRay(Camera* c, int w, int h, float ar, int x, int y);

simd_float3 calcCameraRayDirection(simd_float3 right, simd_float3 up, simd_float3 forward, int i);

void update_camera(Camera *c, float dt, SDL_bool*  keyboard_keys);

#endif /* Camera_h */
