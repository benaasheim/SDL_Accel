//
//  main.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <SDL2/SDL.h>
#include "simd/simd.h"

#include "Settings.h"
#include "Ray.h"
#include "Hit.h"

#include "Tracer.h"

struct
//__attribute__ ((__packed__))
Block {
    SDL_Rect rect;
    Ray ray;
    simd_float3 color;
    int dgd;
};

typedef struct Block Block;

void drawBlocks(void);
void drawBlock(Block block);
void degradeBlock(Block block);
int main(int argc, const char * argv[]);

#endif /* main_h */
