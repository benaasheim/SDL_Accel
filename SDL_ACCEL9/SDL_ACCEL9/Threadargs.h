//
//  Threadargs.h
//  SDL_ACCEL9
//
//  Created by Ben on 8/12/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Threadargs_h
#define Threadargs_h

typedef struct {
    int x_offset;
    int y_offset;
    int width;
    int height;
} threadarg;

void setthreadarg(threadarg* t, i, j) {
    t->height = step_placeholder_y;
    t->width = step_placeholder_x;
    t->x_offset = i * t->width;
    t->y_offset = j * t->height;
}

#endif /* Threadargs_h */

