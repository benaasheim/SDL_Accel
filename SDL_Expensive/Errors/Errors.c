//
//  Errors.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Errors.h"
SDL_Window* window;

void setWindow(SDL_Window* w) {
    window = w;
}

void init_Errors(SDL_Window* w) {
    setWindow(w);
}

void nullErrorCheck(void* p, const char * title) {
    if (p == NULL) {
        SDL_ShowSimpleMessageBox(0, title, SDL_GetError(), window);
        SDL_Quit();
        SDL_DestroyWindow(window);
        exit(0);
    }
}
