//
//  Tracer.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
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

#define float_max 99999999

typedef struct {
    Camera camera;
    Scene scene;
    int thread_amount;
    SDL_bool keyboard_keys[255];
} Tracer;

Tracer newTracer(void (*initmode)(Scene* scene)) {
    Tracer tracer;
    tracer.thread_amount = threads;
    tracer.camera = newCamera(newVector(0, 1, 0), camera_speed, camera_sensitivity);
    tracer.scene = newScene(initmode);
    for (int i = 0; i < 255; i++) {
        tracer.keyboard_keys[i] = SDL_FALSE;
    }
    return tracer;
}

void update_Tracer(Tracer* tracer, double dt) {
//    printf("DT: %f\n", dt);
    update_camera(&tracer->camera, dt, tracer->keyboard_keys);
    update_Scene(&tracer->scene, dt);
}

void handle_key_pressed(SDL_Event event, SDL_bool val, Tracer* tracer) {
    // keyboard API for key pressed
    tracer->keyboard_keys[event.key.keysym.scancode] = val;
}

// Tracing Functions
Hit* basicLoop(Ray *ray, float L_length, Scene* scene, Object* thisobj) {
    //find nearest object intersected by ray
    Hit* fHit = NULL;
    Hit FH;
    Object* obj;
    
    for (int i = 0; i < scene->numObjects; i++) {
        obj = &scene->objects[i];

        Hit* hit = obj->interesect(ray, obj);

        if (hit != NULL &&
            hit->t < L_length &&
            obj != thisobj) {
            
            FH = *hit;
            fHit = &FH;
            L_length = hit->t;
        }
    }
    return fHit;
}

float traceShadow(Ray* ray, Object* thisobj, Scene* scene, Light* light) {
    Hit* xFinal = NULL;
    Material* m;
    
    xFinal = basicLoop(ray, vLength(ray->direction), scene, thisobj);
    
    if (xFinal == NULL) {
        return 1.0f;
    }

    m = &scene->materials[((Object*)(xFinal->obj))->mat];
    float weight = averageVector(m->shading_func(ray, xFinal, light));
    
    if (m->reflectivity > 0.0f) {
        weight -= m->reflectivity;
    }

    if (m->refractivity > 0.0f) {
        weight *= m->refractivity;
    }

    return weight;
}


Vector traceColor(Ray *r, Scene* scene, int n) {
    Vector cFinal = newVector(0.0f, 0.0f, 0.0f);
    Hit* xFinal = NULL;

    if (n > max_recursion) {
        return cFinal;
    }

    //find nearest object intersected by ray
    xFinal = basicLoop(r, float_max, scene, NULL);

    //return black if no objects intersected
    if (xFinal == NULL) {
        return cFinal;
    }

    Ray next_ray;
    Hit XF;
    XF = *xFinal;

    Light* l;

    next_ray.origin = XF.position;
    Material* m = &scene->materials[((Object*)(XF.obj))->mat];

    for (int i = 0; i < scene->numLights; i++) {
        l = &scene->lights[i];

        cFinal = m->shading_func(r, &XF, l);

        if (m->reflectivity != 1.0f) {
            next_ray.direction = normalize(minusVector(l->position, XF.position));
            float first = traceShadow(&next_ray, XF.obj, scene, l) + m->reflectivity;

            cFinal = scaleVector(fminf(first, 1.0f), cFinal);
        }
    }

    if (m->reflectivity > 0.0f) {
        next_ray.direction = normalize(reflect(r->direction, XF.normal));
        cFinal = addVector(cFinal, scaleVector(m->reflectivity, traceColor(&next_ray, scene, n+1)));
    }

    if (m->refractivity > 0.0f) {
        next_ray.direction = normalize(refract(r->direction, XF.normal, m->ior, dot(r->direction, XF.normal)));
        cFinal = addVector(cFinal, scaleVector(m->refractivity, traceColor(&next_ray, scene, n+1)));
    }

    return quickclampVector(addVector(cFinal, m->ambient_color));
}

Vector traceColorSimple(Ray *r, Scene* scene, int n) {

    Vector cFinal = newVector(0, 0, 0);
    Hit* xFinal = NULL;

    if (n > max_recursion) {
        return cFinal;
    }

    //find nearest object intersected by ray
    xFinal = basicLoop(r, float_max, scene, NULL);

    //return black if no objects intersected
    if (xFinal == NULL) {
        return cFinal;
    }

    Ray next_ray;
    Hit XF;
    XF = *xFinal;

    next_ray.origin = XF.position;
    Material* m = &scene->materials[((Object*)(XF.obj))->mat];
    return m->diffuse_color;
}

#endif /* Tracer_h */
