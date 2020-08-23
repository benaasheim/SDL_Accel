//
//  Display.h
//  SDL_ACCEL3
//
//  Created by Ben on 7/28/20.
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
//    SDL_Color* pixels[window_width][window_height];
    Vector pixels[window_width][window_height];
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

#endif /* Display_h */
