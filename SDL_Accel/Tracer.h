//
//  Tracer.h
//  SDL_Accel
//
//  Created by Ben on 7/22/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Tracer_h
#define Tracer_h
#include "Camera.h"
#include "Scene.h"
#include "Settings.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_timer.h>


typedef struct {
    Camera camera;
    Scene scene;
    int thread_amount;
//    Timer timer;
    SDL_bool keyboard_keys[255];
} Tracer;

Tracer newTracer(void (*initmode)(Scene* scene)) {
    Tracer tracer;
    tracer.thread_amount = threads;
    tracer.camera = newCamera(newVector(0, 1, 0), camera_speed, camera_sensitivity);
    tracer.scene = newScene(initmode);
//    tracer.timer = newTimer();
//    tracer.keyboard_keys[SDL_SCANCODE_F] = SDL_FALSE;
    for (int i = 0; i < 255; i++) {
        tracer.keyboard_keys[i] = SDL_FALSE;
    }
    return tracer;
}

void update_Tracer(Tracer* tracer, double dt) {
    printf("DT: %f\n", dt);
    update_camera(&tracer->camera, dt, tracer->keyboard_keys);
    update_Scene(&tracer->scene, dt);
}

void handle_key_pressed(SDL_Event event, SDL_bool val, Tracer* tracer) {
    // keyboard API for key pressed
    tracer->keyboard_keys[event.key.keysym.scancode] = val;
}

#endif /* Tracer_h */
