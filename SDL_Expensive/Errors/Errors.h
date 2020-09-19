//
//  Errors.h
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Errors_h
#define Errors_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "Configuration.h"
#include "ErrorMessages.h"

void init_Errors(SDL_Window* win);
void nullErrorCheck(void* p, const char * title);

#endif /* Errors_h */
