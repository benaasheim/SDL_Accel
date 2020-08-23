//
//  Camera.h
//  SDL_ACCEL6
//
//  Created by Ben on 8/6/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <stdio.h>
#include "Ray.h"
#include "Quaternion.h"
#include "Settings.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define AA(i) ((i%window_width - window_width * 0.5)/window_width*aspect_ratio)
#define BB(i) ((window_height * 0.5 - i/window_width)/window_height)

typedef struct {
    vec position;
    vec rot;
    float speed;
    float sensitivity;
} Camera;

Camera newCamera(vec v, float sp, float se) {
    Camera camera;
    camera.position = v;
    vec r = {0, 1, 0, 0};
    camera.rot = r;
    camera.speed = sp;
    camera.sensitivity = se;
    return camera;
}

void move_camera(Camera* c, float * restrict dir, float amount) {
    mulfv(amount, dir, dir);
    addVec(&c->position, dir, &c->position);
}

void rotate_camera(Camera* c, float * restrict axis, float theta) {
    QuaternionVTSelf(axis, theta);
    mulqq(axis, &c->rot, &c->rot);
    normalizeQSelf(&c->rot);
}

void camera_Forward(Camera* c, float * restrict a) {
    ForwardVector(&c->rot, a);
}

void camera_Right(Camera* c, float * restrict a) {
    RightVector(&c->rot, a);
}

void camera_Up(Camera* c, float * restrict a) {
    UpVector(&c->rot, a);
}

Ray calcCameraRay(Camera* c, int i) {
    Ray ray;

//    ray.origin = camera_Right_Vec(c);
    camera_Right(c, &ray.origin);
    mulfv(AA(i), &ray.origin, &ray.origin);

//    ray.direction = camera_Up_Vec(c);
    camera_Up(c, &ray.direction);
    mulfv(BB(i), &ray.direction, &ray.direction);

    addVec(&ray.origin, &ray.direction, &ray.origin);

//    ray.direction = camera_Forward_Vec(c);
    camera_Forward(c, &ray.direction);
    addVec(&ray.origin, &ray.direction, &ray.direction);

    normalize(&ray.direction, &ray.direction);

    ray.origin = c->position;

    return ray;
}

void update_camera(Camera *c, float dt, SDL_bool*  keyboard_keys) {
    //Camera Movement
    vec forward, right, up;
    camera_Forward(c, &forward);
    camera_Right(c, &right);
    camera_Up(c, &up);
    
    if (keyboard_keys[SDL_SCANCODE_W]) {
        move_camera(c, &forward, c->speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_S]) {
        mulfv(-1, &forward, &forward);
        move_camera(c, &forward, c->speed * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_A]) {
        mulfv(-1, &right, &right);
        move_camera(c, &right, c->speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_D]){
        move_camera(c, &right, c->speed * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_R]) {
        move_camera(c, &up, c->speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_F]){
        mulfv(-1, &up, &up);
        move_camera(c, &up, c->speed * dt);
    }
    
    //Camera Rotation
    camera_Forward(c, &forward);
    camera_Right(c, &right);
    camera_Up(c, &up);
    
    if (keyboard_keys[SDL_SCANCODE_DOWN]) {
        rotate_camera(c, &right, c->sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_UP]) {
        mulfv(-1, &right, &right);
        rotate_camera(c, &right, c->sensitivity * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_RIGHT]) {
        rotate_camera(c, &up, c->sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_LEFT]){
        mulfv(-1, &up, &up);
        rotate_camera(c, &up, c->sensitivity * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_E]) {
        rotate_camera(c, &forward, c->sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_Q]){
        mulfv(-1, &forward, &forward);
        rotate_camera(c, &forward, c->sensitivity * dt);
    }
}


#endif /* Camera_h */
