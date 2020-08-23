//
//  main.c
//  SDL_ACCEL10
//
//  Created by Ben on 8/13/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <SDL2/SDL.h>

#include <stdio.h>
#include "Ray.h"
#include "Light.h"
#include "Settings.h"
#include "Materials.h"
#include "Scene.h"
#include "Tracer.h"

//Globals
Scene* scene;
Camera* camera;
simd_float3* pixels;
SDL_Renderer* renderer;

void sub_display(void *arguments) {
    // Ray-tracing loop, for each pixel
    
    int start = *((int *)arguments);
    int end = start + step_placeholder;
    
    Ray ray;
    ray.origin = camera->position;
    
    simd_float3 right = camera_Right(camera);

    simd_float3 up = camera_Up(camera);

    simd_float3 forward = camera_Forward(camera);
        
    for (int i = start; i < end; i++) {
        ray.direction = forward + (AA(i) * right) + (up * BB(i));
        
        ray.direction = simd_normalize(ray.direction);
        
        pixels[i] = traceColor(ray, scene, 0) * 255;
    }
}

void sub_display2(void *arguments) {
    // Ray-tracing loop, for each pixel
    
    int start = *((int *)arguments);
    int end = start + y_placeholder;
    
    Ray ray;
    ray.origin = camera->position;
    
    simd_float3 right = camera_Right(camera);

    simd_float3 up = camera_Up(camera);

    simd_float3 forward = camera_Forward(camera);
    
    SDL_Rect rect;
    rect.h = resolution;
    rect.w = resolution;
    for (int y = start; y < end; y++) {
        for (int x = 0; x < window_width; x++) {
            ray.direction = calcCameraRayDirection(right, up, forward, y * window_width + x);
            pixels[y * window_width + x] = traceColor(ray, scene, 0) * 255;

        }
    }
}

void handle_threads(pthread_t* thread_lis, int* thread_args) {
    int i;
    //create all the threads one by one
    for (i = 0; i < threads; i++) {
        pthread_create(&thread_lis[i], NULL, sub_display2, &thread_args[i]);
    }
    //wait for each thread to complete
    for (i = 0; i < threads; i++) {
        pthread_join(thread_lis[i], NULL);
    }
}

void handle_threads2(pthread_t* thread_lis, int* thread_args) {
    int i;
    //create all the threads one by one
    for (i = 0; i < threads; i++) {
        pthread_create(&thread_lis[i], NULL, sub_display, &thread_args[i]);
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
    renderer = rend;
    SDL_Rect rect;
    rect.h = resolution;
    rect.w = resolution;
    simd_float3 pix[window_height][window_width];
    pixels = pix;
    simd_float3 col;
    
    //init
    Tracer tracer = newTracer(init_Scene3);
    camera = &tracer.camera;
    scene = &tracer.scene;

    //create all the thread arguements one by one
    for (int i = 0; i < threads; i++) {
        thread_args[i] = i *
//        step_placeholder
        y_placeholder
        ;
    }
    
    for (int i = 0; i < total_pixels; i++) {
        pix[i/window_width][i%window_width] = simd_make_float3(0, 0, 0);
    }
    
    // annimation loop
    int close = 0;
        
    double time_taken;

    clock_t t;
    int x, y;
    
    while (
//           frames < 60
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
//        sub_display2();
        
        
        //update_pixels
        for (int i = 0; i < total_pixels; i++) {
            x = i%window_width;
            y = i/window_width;
            col = pix[y][x];
//            col = pix[i];
            SDL_SetRenderDrawColor(rend, col.x, col.y, col.z, 255);
            rect.x = x
            * resolution
            ;
            rect.y = y
            * resolution
            ;
            SDL_RenderFillRect(rend, &rect);
//            SDL_RenderDrawPoint(rend, x, y);
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

    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(rend);
    return 0;
}
