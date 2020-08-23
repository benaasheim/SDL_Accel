//
//  Quaternion.h
//  SDL_ACCEL8
//
//  Created by Ben on 8/11/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Quaternion_h
#define Quaternion_h

#include "Vector.h"

typedef struct {
    float w[SIZE];
    float x[SIZE];
    float y[SIZE];
    float z[SIZE];
} quat;

void mulqq(float * restrict a, float * restrict q, //input vectors
           float * restrict r //result vector
) {
    int i;
    for (i = 0; i < SIZE; i++) {
        r[i] = a[i+SIZE*3] * q[i] + a[i] * q[i+SIZE*3] + a[i+SIZE] * q[i+SIZE*2] - a[i+SIZE*2] * q[i+SIZE];
    }
    r += SIZE;
    for (i = 0; i < SIZE; i++) {
        r[i] = a[i+SIZE*3] * q[i+SIZE] - a[0] * q[i+SIZE*2] + a[i+SIZE] * q[i+SIZE*3] + a[i+SIZE*2] * q[0];
    }
    r += SIZE;
    for (i = 0; i < SIZE; i++) {
        r[i] = a[i+SIZE*3] * q[i+SIZE*2] + a[i] * q[i+SIZE] - a[i+SIZE] * q[i] + a[i+SIZE*2] * q[i+SIZE*3];
    }
    r += SIZE;
    for (i = 0; i < SIZE; i++) {
        r[i] = a[i+SIZE*3] * q[i+SIZE*3] - a[i] * q[i] - a[i+SIZE] * q[i+SIZE] - a[i+SIZE*2] * q[i+SIZE*2];
    }
}

void mulqv(float * restrict a, float * restrict q, //input vectors
           float * restrict r //result vector
) {
    int i;
    for (i = 0; i < SIZE; i++) {
        r[i] = a[i+SIZE*3] * q[i] + a[i+SIZE] * q[i+SIZE*2] - a[i+SIZE*2] * q[i+SIZE];
    }
    r += SIZE;
    for (i = 0; i < SIZE; i++) {
        r[i] = a[i+SIZE*3] * q[i+SIZE] + a[i+SIZE*2] * q[i] - a[i] * q[i+SIZE*2];
    }
    r += SIZE;
    for (i = 0; i < SIZE; i++) {
        r[i] = a[i+SIZE*3] * q[i+SIZE*2] + a[i] * q[i+SIZE] - a[i+SIZE] * q[i];
    }
    r += SIZE;
    for (i = 0; i < SIZE; i++) {
        r[i] = -a[i] * q[i] - a[i+SIZE] * q[i+SIZE] - a[i+SIZE*2] * q[i+SIZE*2];
    }
}

#endif /* Quaternion_h */
