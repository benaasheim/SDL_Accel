//
//  main.c
//  SDL_Accel
//
//  Created by Ben on 6/25/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "Vector.h"
#include "Object.h"
#include "Settings.h"
#include "Materials.h"
#include "Light.h"
#include "Quaternion.h"
#include "Camera.h"
#include "Scene.h"
#include "Display.h"
#include "Tracer.h"
#include "InputHandler.h"

#define float_max 99999999

//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//    rmask = 0xff000000;
//    gmask = 0x00ff0000;
//    bmask = 0x0000ff00;
//    amask = 0x000000ff;
//#else
//    rmask = 0x000000ff;
//    gmask = 0x0000ff00;
//    bmask = 0x00ff0000;
//    amask = 0xff000000;
//#endif

//Globals
Display* display;
Scene* scene;
Camera* camera;

SDL_Color *create_Color(float x, float y, float z) {
    SDL_Color* col = NULL;
    col = (SDL_Color *)malloc(sizeof(SDL_Color));
    col->r = x;
    col->g = y;
    col->b = z;
    col->a = 255;
    return col;
}

// Tracing Functions
Hit* basicLoop(Ray *ray, float L_length, Object* thisobj) {
    //find nearest object intersected by ray
//    float tInit = float_max;
    Hit* fHit = NULL;
    Hit FH;
//    int hitplane = 0;
    Object* obj;
    
    for (int i = 0; i < scene->numObjects; i++) {
        obj = &scene->objects[i];

        Hit* hit = obj->interesect(ray, obj);

        if (hit != NULL &&
//            hit->t < tInit &&
            hit->t < L_length &&
            obj != thisobj) {
            
            FH = *hit;
            fHit = &FH;
            L_length = hit->t;
        }
    }

    return fHit;
}

float traceShadow(Ray* ray, Object* thisobj, Light* light) {
    Hit* xFinal = NULL;
    Material* m;
    
    xFinal = basicLoop(ray, vLength(ray->direction), thisobj);
    
    if (xFinal == NULL) {
        return 1.0f;
    }
//    xObj = (Object*)(xFinal->obj);
//    m = xObj->mat;
    m = &scene->materials[((Object*)(xFinal->obj))->mat];
    float weight = averageVector(m->shading_func(ray, xFinal, light
//                                                 , *m
                                                 ));
    
    if (m->reflectivity > 0.0f) {
        weight -= m->reflectivity;
    }

    if (m->refractivity > 0.0f) {
        weight *= m->refractivity;
    }

    return weight;
}

Vector traceColor(Ray *r, int n) {
    
    Vector cFinal = newVector(0, 0, 0);
    Hit* xFinal = NULL;

    if (n > max_recursion) {
        return cFinal;
    }

    //find nearest object intersected by ray
    xFinal = basicLoop(r, float_max, NULL);

    //return black if no objects intersected
    if (xFinal == NULL) {
        return cFinal;
    }

    Ray next_ray;
    Hit XF;
    XF = *xFinal;
//    Material* m = &((Object*)(XF.obj))->mat;
    
    Light* l;
    
//    Object* objj = (Object*)(XF.obj);
    
    next_ray.origin = XF.position;
    Material* m = &scene->materials[((Object*)(XF.obj))->mat];
//    printVector(objj->mat->diffuse_color);
    for (int i = 0; i < scene->numLights; i++) {
        l = &scene->lights[i];
        
        cFinal = m->shading_func(r, &XF, l
//                                       , *m
                                        );
        
        if (m->reflectivity != 1.0f) {

//            next_ray = newRay(minusVector(l->position, XF.position), XF.position);
            
            next_ray.direction = normalize(minusVector(l->position, XF.position));
            float first = traceShadow(&next_ray, XF.obj, l) + m->reflectivity;
            
            cFinal = scaleVector(fminf(first, 1.0f), cFinal);
        }
    }

    if (m->reflectivity > 0.0f) {
//        next_ray = newRay(reflect(r->direction, XF.normal), XF.position);
        next_ray.direction = normalize(reflect(r->direction, XF.normal));
        cFinal = addVector(cFinal, scaleVector(m->reflectivity, traceColor(&next_ray, n+1)));
    }
    
    if (m->refractivity > 0.0f) {
//        next_ray = newRay(refract2(r->direction, XF.normal, m->ior, dot(r->direction, XF.normal)), XF.position);
        next_ray.direction = normalize(refract2(r->direction, XF.normal, m->ior, dot(r->direction, XF.normal)));
        cFinal = addVector(cFinal, scaleVector(m->refractivity, traceColor(&next_ray, n+1)));
    }
    
    return quickclampVector(addVector(cFinal, m->ambient_color)
//                            , 0.0f, 1.0f
                            );
}

Vector traceColorAlt(Ray *r, int n) {

    Vector cFinal = newVector(0, 0, 0);
    Hit* xFinal = NULL;

    if (n > max_recursion) {
        return cFinal;
    }

    //find nearest object intersected by ray
    xFinal = basicLoop(r, float_max, NULL);

    //return black if no objects intersected
    if (xFinal == NULL) {
        return cFinal;
    }
    

    Ray next_ray;
    Hit XF;
    XF = *xFinal;
//    Material* m = &((Object*)(XF.obj))->mat;
//    Object* objj = (Object*)(XF.obj);
    
    next_ray.origin = XF.position;
    Material* m = &scene->materials[((Object*)(XF.obj))->mat];
//    printVector(objj->mat->diffuse_color);
    return m->diffuse_color;
}

typedef struct {
    int start;
    int end;
    Camera* camera;
} SD_arg;

SD_arg newSDarg(int start, int end, Camera* camera) {
    SD_arg arg;
    arg.start = start;
    arg.end = end;
    arg.camera = camera;
    return arg;
}

void sub_display(void *arguments) {
    // Ray-tracing loop, for each pixel
    int start = *((int *)arguments);
    int end = start + step_placeholder;
    
//    SDL_Color* col;
//    Vector pixelColor;
    
    Ray ray;
    for (int i = start; i < end; i++) {
//        col = display->pixels[i/window_width][i%window_width];
        ray = calcCameraRay(*camera, window_width, window_height, 1, i%window_width, i/window_width);
//        pixelColor = traceColor(&ray, 0);
//        setPixelColor(i/window_width, i%window_width, pixelColor, display);
        setPixelColorAlt(i, traceColor(&ray, 0), display);
    }
//    for (int i = start; i< end; i++) {
//        for (int j=0; j<window_width; j++) {
//            if (antialias() || depthOfField()) {
////                pixelColor = antialiasPixel(i,j);
//            }
//            else {
//                ray = calcCameraRay(*camera, window_width, window_height, 1, j, i);
//                pixelColor = traceColor(&ray, 0);
//            }
//
//
//            // Update pixel color to result from ray
////            printVector(pixelColor);
//            setPixelColor(i, j, pixelColor, display);
////            col = display->pixels[i][j];
//////            printVector(pixelColor);
////            col->r = pixelColor.x;
////            col->g = pixelColor.y;
////            col->b = pixelColor.z;
//////            printf("%f %f %f\n", pixelColor.r, pixelColor.g, pixelColor.b);
//        }
//    }
}



void handle_threads(pthread_t* thread_lis, int* thread_args,
//                    SDL_Renderer *renderer,
                    Camera* camera) {
    int i;
    
    
    
    int result_code;
//    printf("WindowHieght/threads%d\n", window_height/threads());

    //create all the threads one by one
    for (i = 0; i < threads; i++) {
//        printf("IN MAIN: Creating thread %d.\n", i);
//        thread_args[i].start = i * step_placeholder;
//        thread_args[i].end = i * step_placeholder + step_placeholder;
//        thread_args[i].camera = camera;
//        thread_args[i] = i * step_placeholder;
        result_code = pthread_create(&thread_lis[i], NULL, sub_display, &thread_args[i]);
//        assert(!result_code);
    }

//    printf("IN MAIN: All threads are created.\n\n");

    wait for each thread to complete
    for (i = 0; i < threads; i++) {
        result_code = pthread_join(thread_lis[i], NULL);
        assert(!result_code);
//        printf("IN MAIN: Thread %d has ended.\n", i);
    }

//    printf("MAIN program has ended.\n");
}

void cancel_threads(pthread_t* thread_lis) {
    int i;
    for (i = 0; i < threads; i++) {
            pthread_cancel(thread_lis[i]);
        }
}

// Display method generates the image
void render(SDL_Renderer *renderer, Camera* camera, pthread_t* thread_lis, int* thread_args) {
    if (threads > 1) {
        handle_threads(thread_lis, thread_args,
//                       renderer,
                       camera);
    }
    else {
    }
    SDL_Color* col;
//    for (int i=0; i<window_height; i++) {
//            for (int j=0; j<window_width; j++) {
//                    col = display->pixels[i][j];
////                    SDL_Renderer *r = renderer;
//                    drawPixel(j, i, col, renderer);
//                }
//            }
    for (int i = 0; i < total_pixels; i++) {
        col = display->pixels[i/window_width][i%window_width];
        drawPixel(i%window_width, i/window_width, col, renderer);
    }
//    printf("DISPLAYING\n");
}

int main(int argc, const char * argv[]) {
    unsigned int frames = 0;
    pthread_t thread_lis[threads];
    int thread_args[threads];
    
    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    
    // creates a window
    SDL_Window* win = SDL_CreateWindow("GAME",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        window_width,
                                        window_height,
                                        0);
    
    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_GL_DOUBLEBUFFER;
    
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, 0);
    
    
    
    
    //init
    Tracer tracer = newTracer(init_Scene1);
//    Camera*
    camera = &tracer.camera;
    scene = &tracer.scene;

    Display dsp = newDisplay(win, rend);
    display = &dsp;
    init_display(display);
    double time_taken;
    
    clock_t t;
    // controls annimation loop
    int close = 0;
    
    
    
    
    
    int i;
        
        
        
    int result_code;
//    printf("WindowHieght/threads%d\n", window_height/threads());

    //create all the threads one by one
    for (i = 0; i < threads; i++) {
        thread_args[i] = i * step_placeholder;
//        result_code = pthread_create(&thread_lis[i], NULL, sub_display, &thread_args[i]);
//        assert(!result_code);
    }
    
    
    
    printf("MATERIAL SIZE %d \n", sizeof(
//                                         Material
                                         void*
                                         ));
    
    
    render(rend, camera, thread_lis, thread_args);
    // annimation loop
    while (!close) {
        t = clock();
        SDL_Event event;
        
        // Events mangement
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;
                case SDL_KEYDOWN:
    //                    handle_key_pressed(event, &dest, speed);
                    handle_key_pressed(event, SDL_TRUE, &tracer);
//                    printf("KEY ");
                    break;
                case SDL_KEYUP:
                    handle_key_pressed(event, SDL_FALSE, &tracer);
                    
                    break;
            }
        }

        
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
//        printf("fun() took %f seconds to execute \n", time_taken);
        //updates tracer
        update_Tracer(&tracer, time_taken);
        
        // clears the screen
//        SDL_RenderClear(rend);
//        SDL_RenderCopy(rend, tex, NULL, &dest);

        
        t = clock();
//        render(rend, camera, thread_lis, thread_args);
//        frames++;
        
//        printf("Frames: %d\n", frames);

        
        // triggers the double buffers
        // for multiple rendering
        cancel_threads(thread_lis);
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        SDL_RenderPresent(rend);
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        render(rend, camera, thread_lis, thread_args);

    //        // calculates to 60 fps
    //        SDL_Delay(1000 / 60);
    }

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);
    return 0;
}
