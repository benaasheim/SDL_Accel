//
//  Display.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Display_h
#define Display_h
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_timer.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Color* pixels[window_width][window_height];
} Display;

SDL_Rect rect;

Display newDisplay(SDL_Window* window, SDL_Renderer* renderer) {
    Display display;
    display.window = window;
    display.renderer = renderer;
    rect.h = resolution;
    rect.w = resolution;
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
    for (int i = 0; i < window_height; i++) {
        for (int j = 0; j < window_width; j++) {
            display->pixels[i][j] = create_Color_empty();
        }
    }
}

void drawPixel(int i, int j, SDL_Color *color, SDL_Renderer *renderer) {
    if (resolution > 1) {
        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
//        SDL_RenderDrawPoint(renderer, i*resolution+a, j*resolution+b);
        rect.x = i * resolution;
        rect.y = j * resolution;
//        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
        SDL_RenderDrawPoint(renderer, i, j);
    }
}

void drawPixel2(int i, int j, Vector color, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
    if (resolution > 1) {
//        SDL_RenderDrawPoint(renderer, i*resolution+a, j*resolution+b);
        rect.x = i * resolution;
        rect.y = j * resolution;
//        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }
    else {
        SDL_RenderDrawPoint(renderer, i, j);
    }
}

void setPixelColor_(int i, Vector vec, Display* display) {
    vec = quickclampVector(vec);
    vec = scaleVector(255, vec);
    SDL_Color** coll = display->pixels;
    SDL_Color* col = coll[i];
    col->r = vec.x;
    col->g = vec.y;
    col->b = vec.z;
}

void update_display(Display* display) {
    SDL_Color* col;
    for (int i = 0; i < total_pixels; i++) {
        col = display->pixels[i/window_width][i%window_width];
        drawPixel(i%window_width, i/window_width, col, display->renderer);
    }
//    SDL_RenderPresent(display->renderer);
}

#endif /* Display_h */
