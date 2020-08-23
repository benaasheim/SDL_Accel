//
//  Vector.h
//  SDL_Accel
//
//  Created by Ben on 7/21/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Vector_h
#define Vector_h

typedef struct {
    float x;
    float y;
    float z;
} Vector;

// Create a new Vector with the given values
Vector newVector(float x, float y, float z) {
    Vector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

void printVector(Vector v) {
    printf("V:{%f,%f,%f}\n", v.x, v.y, v.z);
}

// Compute the magnitude of a vector
float mag(Vector v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

// Compute the dot product of two vectors
float dot(Vector a, Vector b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

// Scale a vector by a constant value
Vector scaleVector(float value, Vector v) {
    return newVector(v.x * value, v.y * value, v.z * value);
}

// Scale a vector by a constant value
Vector scaleV(Vector value, Vector v) {
    return newVector(v.x * value.x, v.y * value.y, v.z * value.z);
}

// Compute the cross-product of two vectors
Vector cross(Vector a, Vector b) {
    return newVector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

// Compute the addition of two vectors
Vector addVector(Vector a, Vector b) {
    return newVector(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Compute a vector produced by substracting a vector from another
Vector minusVector(Vector a, Vector b) {
    return addVector(a, scaleVector(-1.0f,b));
}

// Scale a vector by a constant value
Vector divideFloat(float value, Vector v) {
    return newVector(v.x / value, v.y / value, v.z / value);
}

float vLength(Vector v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector normalize(Vector v) {
    return divideFloat(vLength(v), v);
}

Vector reflect(Vector a, Vector b) {
    return minusVector(b, scaleVector(2, scaleVector(dot(a, b), a)));
}

Vector refract(Vector a, Vector b, float n, float NdotI, float cos_t) {
    cos_t = sqrtf(1.0f-cos_t);
    if (cos_t < 1.0f) {
        return addVector(scaleVector(n, a), scaleVector(n * NdotI - cos_t, b));
    }
    else {
        return reflect(a, b);
    }
}

Vector refract2(Vector a, Vector b, float ior, float NdotI) {
    float cos_t;
    if (NdotI > 0.0f) {
        b = scaleVector(-1.0f, b);
    }
    else {
        ior = 1.0f / ior;
        NdotI = -NdotI;
    }
    cos_t = sqrtf(1.0f - ior * ior * (1.0f - NdotI * NdotI));
    if (cos_t < 1.0f) {
        return addVector(scaleVector(ior, a), scaleVector(ior * NdotI - cos_t, b));
    }
    else {
        return reflect(a, b);
    }
}

float clamp(float f, float min, float max) {
    printf("LOL");
    return fmaxf(min, fminf(f, max));
}

float quickclamp(float f) {
    return fmaxf(0.0f, fminf(f, 1.0f));
}

Vector clampVector(Vector v, float min, float max) {
    printf("LOL");
    float x, y, z;
    x = clamp(v.x, min, max);
    y = clamp(v.y, min, max);
    z = clamp(v.z, min, max);
    return newVector(x, y, z);
}

Vector quickclampVector(Vector v) {
    float x, y, z;
    x = quickclamp(v.x);
    y = quickclamp(v.y);
    z = quickclamp(v.z);
    return newVector(x, y, z);
}

float smoothstep(float f, float min, float max) {
    return
//    clamp((f-min)/(max-min), 0.0f, 1.0f)
    quickclamp((f-min)/(max-min))
    ;
}

float quickstep(float f, float min, float max) {
    return
//    clamp((f-min)/(max-min), 0.0f, 1.0f)
//    quickclamp((f-min)/(max-min))
    quickclamp(f)
    ;
}

Vector smoothstepVector(Vector v, float min, float max) {
    float x = smoothstep(v.x, min, max);
    float y = smoothstep(v.y, min, max);
    float z = smoothstep(v.z, min, max);
    return newVector(x, y, z);
}
//
//Vector quickstepVector(Vector v) {
//    float x = quickclamp(<#float f#>)(v.x, min, max);
//    float y = smoothstep(v.y, min, max);
//    float z = smoothstep(v.z, min, max);
//    return newVector(x, y, z);
//}


float averageVector(Vector v) {
    return (v.x + v.y + v.z)/3.0f;
}

#endif /* Vector_h */
