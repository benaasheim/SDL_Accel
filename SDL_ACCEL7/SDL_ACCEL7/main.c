//
//  main.c
//  SDL_ACCEL7
//
//  Created by Ben on 8/8/20.
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

#include <stdio.h>
#include "Vector.h"
#include "Ray.h"
#include "Quaternion.h"
#include "Hit.h"
#include "Light.h"
#include "Materials.h"
#include "Scene.h"
#include "Tracer.h"

//Globals
Scene* scene;
Camera* camera;
vec* pixels;

void sub_display(void *arguments) {
    // Ray-tracing loop, for each pixel
    
    int start = *((int *)arguments);
    int end = start + step_placeholder;
    
    Ray ray;
    ray.origin = camera->position;
    
    vec right;
    camera_Right(camera, &right);
    
    vec up;
    camera_Up(camera, &up);
    
    vec forward;
    camera_Forward(camera, &forward);
    
    for (int i = start; i < end; i++) {
        vec temp;
        mulfv(AA(i), &right, &temp);
//        mulfvSelf(AA(i), &right);

//        vec temp2;
        mulfv(BB(i), &up, &ray.direction);

//        addVec(&temp, &ray.direction, &ray.direction);
        addVecSelf(&temp, &ray.direction);

//        addVec(&ray.direction, &forward, &ray.direction);
        addVecSelf(&forward, &ray.direction);
        
//        normalize(&ray.direction, &ray.direction);
        normalizeSelf(&ray.direction);
//        ray = calcCameraRay(*camera, i);

        vec tempcol = traceColor(ray, scene, 0);
//        vec tempcol = traceColorSimple(ray, scene, 0);
        mulfvSelf(255.0f, &tempcol);
        pixels[i] = tempcol;
    }
}

void handle_threads(pthread_t* thread_lis, int* thread_args) {
    int i;
    //create all the threads one by one
    for (i = 0; i < threads; i++) {
        pthread_create(&thread_lis[i], NULL, sub_display, &thread_args[i]);
    }
    //wait for each thread to complete
    for (i = 0; i < threads; i++) {
        pthread_join(thread_lis[i], NULL);
    }
}

void cancel_threads(pthread_t* thread_lis) {
    for (int i = 0; i < threads; i++) {
        pthread_cancel(thread_lis[i]);
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
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
                                        dwidth,
                                        dheight,
                                        0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    frames = 0;
    
    SDL_Rect rect;
    rect.h = resolution;
    rect.w = resolution;
    vec pix[window_width][window_height];
    pixels = pix;
    vec col = {0, 0, 0, 0};
    
    //init
    Tracer tracer = newTracer(init_Scene2);
    camera = &tracer.camera;
    scene = &tracer.scene;

    //create all the thread arguements one by one
    for (int i = 0; i < threads; i++) {
        thread_args[i] = i * step_placeholder;
    }
    
    // annimation loop
    int close = 0;
        
    double time_taken;

    clock_t t;
    int x, y;
    
    for (int i = 0; i < total_pixels; i++) {
        x = i%window_width;
        y = i/window_width;
        pix[y][x] = col;
    }
    
    while (
//           frames < 500
           !close
           ) {
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
                    // keyboard API for key pressed
                    tracer.keyboard_keys[event.key.keysym.scancode] = SDL_TRUE;
                    break;
                case SDL_KEYUP:
                    tracer.keyboard_keys[event.key.keysym.scancode] = SDL_FALSE;
                    break;
                }
            }
        
        //updates tracer
        handle_threads(thread_lis, thread_args);
        
        
        //update_pixels
        for (int i = 0; i < total_pixels; i++) {
            x = i%window_width;
            y = i/window_width;
            col = pix[y][x];
            SDL_SetRenderDrawColor(rend, col.x, col.y, col.z, 255);
//            if (resolution > 1) {
                rect.x = x * resolution;
                rect.y = y * resolution;
                SDL_RenderFillRect(rend, &rect);
//            }
//            else {
//                SDL_RenderDrawPoint(rend, x * resolution, y * resolution);
//            }
        }
            
        SDL_RenderPresent(rend);
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
            
        //update tracer stuff
        update_camera(camera, time_taken, tracer.keyboard_keys);
        update_Scene(scene, time_taken);
        
        frames++;
        printf("Frames: %d\n", frames);
    }

    //end threads
    cancel_threads(thread_lis);

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(rend);
    return 0;
}
