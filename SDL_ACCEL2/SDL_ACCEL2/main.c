//
//  main.c
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
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

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif


//Globals
Display* display;
Scene* scene;
Camera* camera;

void sub_display(void *arguments) {
    // Ray-tracing loop, for each pixel
    int start = *((int *)arguments);
    int end = start + step_placeholder;
    Ray ray;
    for (int i = start; i < end; i++) {
        ray = calcCameraRay(*camera, window_width, window_height, 1, i%window_width, i/window_width);
        setPixelColor_(i, traceColor(&ray, scene, 0), display);
    }
}

void sub_display2(void *arguments) {
    // Ray-tracing loop, for each pixel
    int start = *((int *)arguments);
    int end = start + step_placeholder;
    printf("Start: %d End: %d \n", start, end);
    Ray ray;
    double time_taken;

    clock_t t;
    while (1) {
        t = clock();
        for (int i = start; i < end; i++) {
            ray = calcCameraRay(*camera, window_width, window_height, 1, i%window_width, i/window_width);
            setPixelColor_(i, traceColor(&ray, scene, 0), display);
        }
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("Startind: %d, thread took %f\n", start, time_taken);
    }
}

void sub_display3(void *arguments) {
    // Ray-tracing loop, for each pixel
    SDL_Surface* bitmapSurface = SDL_CreateRGBSurface(0, dwidth, dheight, 32, rmask, gmask, bmask, amask);

    int start = *((int *)arguments);
    int end = start + step_placeholder;
    printf("Start: %d End: %d \n", start, end);
    Ray ray;
    while (1) {
        for (int i = start; i < end; i++) {
            ray = calcCameraRay(*camera, window_width, window_height, 1, i%window_width, i/window_width);
//            setPixelColor_(i, traceColor(&ray, scene, 0), display);
//            bitmapSurface->pixels[i] = ;
        }
        
    }
}

void handle_threads(pthread_t* thread_lis, int* thread_args) {
    int i;
    int result_code;
    //create all the threads one by one
    for (i = 0; i < threads; i++) {
        result_code = pthread_create(&thread_lis[i], NULL, sub_display2, &thread_args[i]);
    }
}

void handle_threads2(pthread_t* thread_lis, int* thread_args) {
    int i;
    int result_code;
    //create all the threads one by one
    for (i = 0; i < threads; i++) {
        result_code = pthread_create(&thread_lis[i], NULL, sub_display2, &thread_args[i]);
    }
}

void cancel_threads(pthread_t* thread_lis) {
    for (int i = 0; i < threads; i++) {
        pthread_cancel(thread_lis[i]);
    }
}

// Display method generates the image
void render(SDL_Renderer *renderer, pthread_t* thread_lis, int* thread_args) {
    handle_threads(thread_lis, thread_args);
}

void render2(SDL_Renderer *renderer, pthread_t* thread_lis, int* thread_args) {
    handle_threads(thread_lis, thread_args);
}

//void input_thread(void *arguments) {
//    Tracer* tracer = (Tracer *)arguments;
//
//    double time_taken;
//
//    clock_t t;
//    // Ray-tracing loop, for each pixel
//    while (1) {
//
//
//        t = clock() - t;
//        time_taken = ((double)t)/CLOCKS_PER_SEC;
//        //updates tracer
//        update_Tracer(tracer, time_taken);
//        t = clock();
//
//        SDL_Delay(1000/60);
//    }
//
//}

void display_thread(void *arguments) {
    // Ray-tracing loop, for each pixel
    while (1) {
        update_display(display);
    }
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
                                        dwidth,
                                        dheight,
                                        0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    
    
    Display dsp = newDisplay(win, rend);
    display = &dsp;
    init_display(display);

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
    
    render(rend, thread_lis, thread_args);
    while (!close) {
        SDL_Event event;
        
        // Events mangement
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;
                case SDL_KEYDOWN:
                    handle_key_pressed(event, SDL_TRUE, &tracer);
                    break;
                case SDL_KEYUP:
                    handle_key_pressed(event, SDL_FALSE, &tracer);
                    break;
            }
        }
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        //updates tracer
        
        update_Tracer(&tracer, time_taken);
        t = clock();
        
        
        
        update_display(display);
        SDL_RenderPresent(rend);
    }
    
    //end threads
    cancel_threads(thread_lis);
//    pthread_cancel(d_thread);

    // destroy renderer
    SDL_DestroyRenderer(display->renderer);

    // destroy window
    SDL_DestroyWindow(display->window);
    return 0;
}
