//
//  Tracer.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Tracer_h
#define Tracer_h

#include <stdio.h>
#include "Camera.h"
#include "Scene.h"
#include "Settings.h"

#include <SDL2/SDL.h>

#define float_max 99999999

typedef struct {
    Camera camera;
    Scene scene;
    SDL_bool keyboard_keys[255];
} Tracer;

Tracer newTracer(void (*initmode)(Scene* scene));

void update_Tracer(Tracer* tracer, double dt);

void handle_key_pressed(SDL_Event event, SDL_bool val, Tracer* tracer);

float traceShadow(Ray ray, Object* thisobj, Scene* scene, Light* light, float L_length);

simd_float3 traceColor(Ray ray, Scene* scene, int n, int* dgd);

#endif /* Tracer_h */
