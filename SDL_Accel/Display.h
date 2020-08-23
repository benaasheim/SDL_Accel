//
//  Display.h
//  SDL_Accel
//
//  Created by Ben on 7/22/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Display_h
#define Display_h
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define window_width 320
#define window_height 320

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Color* pixels[window_width][window_height];
} Display;

Display newDisplay(SDL_Window* window, SDL_Renderer* renderer) {
    Display display;
    display.window = window;
    display.renderer = renderer;
    return display;
}

SDL_Color *create_Color_empty() {
    SDL_Color* col = NULL;
    col = (SDL_Color *)malloc(sizeof(SDL_Color));
    col->r = 0;
    col->g = 0;
    col->b = 0;
    col->a = 255;
    return col;
}

void init_display(Display* display) {
    // Initialize empty pixels
    printf("Size of display: %d\n", sizeof(Display));
    for (int i = 0; i < window_height; i++) {
        for (int j = 0; j < window_width; j++) {
            display->pixels[i][j] = create_Color_empty();
        }
    }
}

void drawPixel(int i, int j, SDL_Color *color, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    SDL_RenderDrawPoint(renderer, i, j);
}

void setPixelColor(int i, int j, Vector vec, Display* display) {
    if (i < 0.0f || i > window_width || j < 0.0f || j > window_height) {
        return;
    }
    
    vec = quickclampVector(vec
//                      , 0.0f, 1.0f
                      );
    vec = smoothstepVector(vec, 0.0f, 1.0f);
    vec = scaleVector(255, vec);
//    printVector(vec);
    SDL_Color* col = display->pixels[i][j];
    //            printVector(pixelColor);
    col->r = vec.x;
    col->g = vec.y;
    col->b = vec.z;
}

void setPixelColorAlt(int i, Vector vec, Display* display) {
//    if (i < 0.0f || i > window_width || j < 0.0f || j > window_height) {
//        return;
//    }
    
    vec = quickclampVector(vec
//                           , 0.0f, 1.0f
                           );
//    vec = smoothstepVector(vec, 0.0f, 1.0f);
    vec = scaleVector(255, vec);
//    printVector(vec);
    SDL_Color** coll = display->pixels;
    SDL_Color* col = coll[i];
    //            printVector(pixelColor);
    col->r = vec.x;
    col->g = vec.y;
    col->b = vec.z;
}

#endif /* Display_h */
