//
//  main.c
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "main.h"

Scene* scene;
Camera* camera;
SDL_Rect rectangles[total_pixels * 2];
simd_float3 colors[total_pixels * 2];
int currind;

simd_float3 right, up, forward;

int get_New_Blocks(Block block) {
    return 3*block.dgd;
}

void drawBlocks() {
    currind = 0;
    
    right = camera_Right(camera);

    up = camera_Up(camera);

    forward = camera_Forward(camera);
    
    Block block;
    
    block.rect.x = 0;
    block.rect.y = 0;
    block.rect.h = window_height;
    block.rect.w = window_width;
    
    block.dgd = 0;
    
    drawBlock(block);
//    degradeBlock(block);
    return;
}

void initial_degrade(Block block) {
    int newblocks = get_New_Blocks(block);
    
    if (
        block.rect.w > block.dgd
        &&
        block.rect.h > block.dgd
        &&
        currind < total_pixels
        ) {
        
        
        block.rect.w = block.rect.w/newblocks;
        block.rect.h = block.rect.h/newblocks;
        
        
        Block next;
        next.rect.w = block.rect.w;
        next.rect.h = block.rect.h;
        
        
        for (int x = newblocks/2; x < newblocks/2; x++) {
            for (int y = newblocks/2; y < newblocks/2; y++) {
                if (x == 0 && y == 0) {
                    degradeBlock(block);
                }
                else {
                    next.rect.x = block.rect.x + block.rect.w * x;
                    next.rect.y = block.rect.y + block.rect.h * y;
                    drawBlock(next);
                }
            }
        }
        
    }
    
    return;
}

void degradeBlock(Block block) {
    int newblocks = get_New_Blocks(block);
    
    if (
        block.rect.w > block.dgd
        &&
        block.rect.h > block.dgd
        &&
        currind < total_pixels
        ) {
        
        
        block.rect.w = block.rect.w/newblocks;
        block.rect.h = block.rect.h/newblocks;
        
        
        Block next;
        next.rect.w = block.rect.w;
        next.rect.h = block.rect.h;
        
        
        for (int x = 0; x < newblocks; x++) {
            for (int y = 0; y < newblocks; y++) {
                if (x == 0 && y == 0) {
                    degradeBlock(block);
                }
                else {
                    next.rect.x = block.rect.x + block.rect.w * x;
                    next.rect.y = block.rect.y + block.rect.h * y;
                    drawBlock(next);
                }
            }
        }
    }
    
    return;
}

void drawBlock(Block block) {
    block.ray.direction = calcCameraRayDirection(right, up, forward, block.rect.y * window_width + block.rect.x);
    
    block.ray.origin = camera->position;
    
    block.color = traceColor(block.ray, scene, 0, &block.dgd) * 255;
    
    //draw original
    colors[currind] = block.color;
    rectangles[currind] = block.rect;
    
    currind++;
    degradeBlock(block);
    
    return;
}

void drawPixels(SDL_Renderer* rend) {
    SDL_Rect rectangle;
    for (int i = 0; i < currind; i++) {
        simd_float3 col = colors[i];
        rectangle = rectangles[i];
        rectangle.x *= resolution;
        rectangle.y *= resolution;
        rectangle.h *= resolution;
        rectangle.w *= resolution;
        SDL_SetRenderDrawColor(rend, col.x, col.y, col.z, 255);
        SDL_RenderFillRect(rend, &rectangle);
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    unsigned int frames = 0;
    
    
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
    
    //init
    Tracer tracer = newTracer(init_Scene3);
    camera = &tracer.camera;
    scene = &tracer.scene;
    
    
    //init pixles
    for (int i = 0; i < total_pixels; i++) {
        colors[i] = (simd_float3){0, 0, 0};
    }
    
    // annimation loop
    int close = 0;
    
    double time_taken;
    
    clock_t t;
    
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
        
        //draw pixels
        update_camera(camera, time_taken, tracer.keyboard_keys);
        update_Scene(scene, time_taken);
        SDL_RenderClear(rend);
        drawBlocks();
        drawPixels(rend);
        
        //updates tracer
        SDL_RenderPresent(rend);
        
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        
        //update tracer stuff
        frames++;
        printf("Frames: %d\n", frames);
    }
    
    // destroy renderer
    SDL_DestroyRenderer(rend);
    
    // destroy window
    SDL_DestroyWindow(rend);
    
    return 0;
}
