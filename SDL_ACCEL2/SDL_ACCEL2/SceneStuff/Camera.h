//
//  Camera.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Camera_h
#define Camera_h
#include "Ray.h"
#include "Quaternion.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_timer.h>

typedef struct {
    Vector position;
    Quaternion rot;
    float speed;
    float sensitivity;
} Camera;

Camera newCamera(Vector v, float sp, float se) {
    Camera camera;
    camera.position = v;
    camera.rot = newQuaternion(0, 0, 1, 0);
    camera.speed = sp;
    camera.sensitivity = se;
    return camera;
}

void move_camera(Camera* c, Vector dir, float amount) {
    c->position = addVector(c->position, scaleVector(amount, dir));
}

void rotate_camera(Camera* c, Vector axis, float theta) {
    c->rot = normalizeQ(multQuat(newQuaternionVT(axis, theta), c->rot));
}

Vector camera_Forward_Vec(Camera c) {
    return getForwardVector(c.rot);
}

Vector camera_Right_Vec(Camera c) {
    return getRightVector(c.rot);
}

Vector camera_Up_Vec(Camera c) {
    return getUpVector(c.rot);
}

Ray calcCameraRay(Camera c, int w, int h, float ar, int x, int y) {
    Ray ray;
    float aa = (x - w * 0.5)/w*ar;
    float bb = (h * 0.5 - y)/h;

    Vector a = scaleVector(aa, camera_Right_Vec(c));
    Vector b = scaleVector(bb, camera_Up_Vec(c));
    Vector cc = camera_Forward_Vec(c);
    ray = newRay(addVector(addVector(a, b), cc), c.position);
    return ray;
}

void update_camera(Camera *c, float dt, SDL_bool*  keyboard_keys) {
    //Camera Movement
    if (keyboard_keys[SDL_SCANCODE_W]) {
        move_camera(c, camera_Forward_Vec(*c), c->speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_S]) {
        move_camera(c, scaleVector(-1, camera_Forward_Vec(*c)), c->speed * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_A]) {
        move_camera(c, scaleVector(-1, camera_Right_Vec(*c)), c->speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_D]){
        move_camera(c, camera_Right_Vec(*c), c->speed * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_R]) {
        move_camera(c, camera_Up_Vec(*c), c->speed * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_F]){
        move_camera(c, scaleVector(-1, camera_Up_Vec(*c)), c->speed * dt);
    }
    
    //Camera Rotation
    if (keyboard_keys[SDL_SCANCODE_DOWN]) {
        rotate_camera(c, camera_Right_Vec(*c), c->sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_UP]) {
        rotate_camera(c, scaleVector(-1, camera_Right_Vec(*c)), c->sensitivity * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_RIGHT]) {
        rotate_camera(c, camera_Up_Vec(*c), c->sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_LEFT]){
        rotate_camera(c, scaleVector(-1, camera_Up_Vec(*c)), c->sensitivity * dt);
    }
    if (keyboard_keys[SDL_SCANCODE_E]) {
        rotate_camera(c, camera_Forward_Vec(*c), c->sensitivity * dt);
    }
    else if (keyboard_keys[SDL_SCANCODE_Q]){
        rotate_camera(c, scaleVector(-1, camera_Forward_Vec(*c)), c->sensitivity * dt);
    }
}

#endif /* Camera_h */
