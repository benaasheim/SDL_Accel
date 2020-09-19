//
//  main.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "Scene/Scene.h"
#include "Scene/Cam.h"

#include "SCU/Settings.h"
#include "SCU/InputHandler.h"

#include "Tracing/Tracing.h"

#include "Errors/Errors.h"

void _init(SDL_Window* win, Uint32 * p, SDL_PixelFormat* S) {
    //init
    init_Errors(win);
    initKeyboardKeys();
    init_Scene();
    init_Cam((simd_float3){0, 1, 0}, p, S);
}

void set_threadargs(pthread_t* thread_lis, int* thread_args) {
    int startind = 0;
    int currind  = 0;
    int done = 0;
    while (!done) {
        thread_args[currind] = startind;
        startind +=  y_placeholder;
        thread_args[currind+1] = startind;
        if (y_placeholder + startind >= window_height || currind >= threads*2) {
            done = 1;
        }
        else {
            currind += 2;
        }
    }
    thread_args[currind-1] = window_height;
}

void handle_threads(pthread_t* thread_lis, int* thread_args) {
    int i;
    //create all the threads one by one
    for (i = 0; i < threads; i++) {
        pthread_create(&thread_lis[i], NULL, cast_rays, &thread_args[i*2]);
    }
    
    //wait for each thread to complete
    for (i = 0; i < threads; i++) {
        pthread_join(thread_lis[i], NULL);
    }
}



int main(int argc, const char * argv[]) {
    // insert code here...
    unsigned int frames = 0;
    pthread_t thread_lis[threads];
    int thread_args[threads * 2];
        
    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    
    // creates a window
    SDL_Window* win = SDL_CreateWindow("GAME",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        dwidth,
                                        dheight,
                                        0);
    
    // retutns zero on success else non-zero
    if (win == NULL) {
        printf("error initializing WINDOW: %s\n", SDL_GetError());
    }
    

    
#if resolution == 1
#else
    SDL_Rect rect;
    rect.h = resolution;
    rect.w = resolution;
    
#endif
    
    set_threadargs(thread_lis, thread_args);
    
    //get screen and screen format pointers
    SDL_Surface* screen = SDL_GetWindowSurface(win);
    nullErrorCheck(screen, screen_null_err);
    
    
    SDL_PixelFormat* screenformat = screen->format;
    nullErrorCheck(screenformat, screen_format_null_err);
    
    _init(win, ((Uint32*)screen->pixels), screenformat);
    
    //Set screen blank at first
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_UpdateWindowSurface(win);
    
    
    // annimation loop
    int close = 0;
    
    double time_taken;
    
    clock_t t, tot = clock();
    
    while (
           !close
//           frames < 500
           ) {
        
        t = clock(); //start clock
        
        // Events mangement
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;
                case SDL_KEYDOWN:
                    // keyboard API for key pressed
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                        close = 1;
                        break;
                    }
                    keyboard_keys[event.key.keysym.scancode] = 1;
                    break;
                case SDL_KEYUP:
                    keyboard_keys[event.key.keysym.scancode] = 0;
                    break;
            }
        }
        
        //cast&trace rays
        handle_threads(thread_lis, thread_args);
        
        SDL_UpdateWindowSurface(win);
        
        //update tracer stuff
        
        t = clock() - t; //stop clock
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        update_cam(time_taken, keyboard_keys);
        
        //update frames
        frames++;
        printf("frames: %d\n", frames);
    }
    
    tot = clock() - tot;
    time_taken = ((double)tot)/CLOCKS_PER_SEC;
    printf("TIME: %f\n", time_taken);
    
    // destroy window
    SDL_Quit();
    SDL_DestroyWindow(win);
    
    return 0;
}
