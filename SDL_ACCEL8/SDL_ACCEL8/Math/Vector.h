//
//  Vector.h
//  SDL_ACCEL8
//
//  Created by Ben on 8/11/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Vector_h
#define Vector_h

#include <stdio.h>
#include <math.h>

#define SIZE (1L << 2)

typedef struct {
    float x[SIZE];
    float y[SIZE];
    float z[SIZE];
} vec;

void printRow(char coord, float * restrict a) {
    printf("{");
    for (int i = 0; i < SIZE -1; i++) {
        printf("%f,", a[i]);
    }
    printf("%f}\n", a[SIZE-1]);
}

void printVec(vec v) {
    printf("Vector:XYZ\n");
    printRow('X', &v.x);
    printRow('Y', &v.y);
    printRow('Z', &v.z);
}



void printVecI(vec v, int i) {
    printf("V:{%f,%f,%f}\n", v.x[i], v.y[i], v.z[i]);
}

//vec pointer is passed and all three coordinates are added as a 3xlonger 1d array
//a is 1st vec pointer, b is second, c is result
void addVec(float * restrict a, float * restrict b, //inputs
            float * restrict c //result vector
) {
    for (int i = 0; i < SIZE * 3; i++) {
        c[i] = b[i] + a[i];
    }
}

//vec pointer is passed and all three coordinates are subtracted as a 3xlonger 1d array
//a is 1st vec pointer, b is second, c is result
void subVec(float * restrict a, float * restrict b, //inputs
            float * restrict c //result vector
) {
    for (int i = 0; i < SIZE * 3; i++) {
        c[i] = a[i] - b[i];
    }
}

//vec pointer is passed and all three coordinates are multiplied as a 3x longer 1d array
//a is 1st vec pointer, b is second, c is result
void mulvv(float * restrict a, float * restrict b, //input vectors
           float * restrict c //result vector
) {
    for (int i = 0; i < SIZE * 3; i++) {
        c[i] = b[i] * a[i];
    }
}

void mulffv(float * restrict f, float * restrict a, //inputs
           float * restrict b //result vector
) {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < SIZE; i++) {
            b[i+SIZE*j] = a[i+SIZE*j] * f[i];
        }
    }
}

void mulfv(float f, float * restrict a, //inputs
           float * restrict b //result vector
) {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < SIZE; i++) {
            b[i+SIZE*j] = a[i+SIZE*j] * f;
        }
    }
}

//vec pointer is passed and all three coordinates are multiplied as a 3x longer 1d array
//a is 1st vec pointer, b is second, c is result
void divideFloat(float * restrict a, float * restrict f, //input vectors
                 float * restrict b //result vector
) {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < SIZE; i++) {
            b[i+SIZE*j] = a[i+SIZE*j] / f[i];
        }
    }
}

//each coordinate array's list is passed as x, y, &z and summed into the result float array "sum"
void sumVec(float * restrict x, float * restrict y, float * restrict z, float * restrict sum
) {
    for (int i = 0; i < SIZE; i++) {
        sum[i] = x[i] + y[i] + z[i];
    }
}

void dot(float * restrict a, float * restrict b, float * restrict sum
) {
    vec c;
    mulvv(a, b, &c);
    sumVec(&c.x, &c.y, &c.z, sum);
}

void mag(float * restrict a, float * restrict sum) {
    dot(a, a, sum);
    for (int i = 0; i < SIZE; i++) {
        sum[i] = sqrtf(sum[i]);
    }
}

void normalize(float * restrict a, //input vector
               float * restrict b //result vector
) {
    mag(a, b);
    float f[SIZE];
    divideFloat(b, f, b);
}

void reflect(float * restrict a, float * restrict b, //inputs
             float * restrict c //result vector
) {
    float f[SIZE];
    dot(&a, &b, &f);
    
}

#endif /* Vector_h */
