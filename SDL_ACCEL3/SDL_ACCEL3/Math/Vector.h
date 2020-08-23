//
//  Vector.h
//  SDL_ACCEL3
//
//  Created by Ben on 7/28/20.
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
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Scale a vector by a constant value
Vector scaleVector(float value, Vector v) {
//    return newVector(v.x * value, v.y * value, v.z * value);
    v.x *= value;
    v.y *= value;
    v.z *= value;
    return v;
}

// Scale a vector by a constant value
Vector scaleV(Vector a, Vector b) {
//    return newVector(v.x * value.x, v.y * value.y, v.z * value.z);
    b.x = a.x * b.x;
    b.y = a.y * b.y;
    b.z = a.z * b.z;
    return b;
}

// Compute the cross-product of two vectors
//Vector cross(Vector a, Vector b) {
////return newVector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
//    b.x = a.x + b.x;
//    b.y = a.y + b.y;
//    b.z = a.z + b.z;
//    return b;
//}

// Compute the addition of two vectors
Vector addVector(Vector a, Vector b) {
//    return newVector(a.x + b.x, a.y + b.y, a.z + b.z);
    b.x = a.x + b.x;
    b.y = a.y + b.y;
    b.z = a.z + b.z;
    return b;
}

// Compute a vector produced by substracting a vector from another
Vector minusVector(Vector a, Vector b) {
//    return addVector(a, scaleVector(-1.0f,b));
    b.x = a.x - b.x;
    b.y = a.y - b.y;
    b.z = a.z - b.z;
    return b;
}

// Scale a vector by a constant value
Vector divideFloat(float value, Vector v) {
//    return newVector(v.x / value, v.y / value, v.z / value);
    v.x /= value;
    v.y /= value;
    v.z /= value;
    return v;
}

Vector normalize(Vector v) {
//    return divideFloat(mag(v), v);
    float value = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    v.x /= value;
    v.y /= value;
    v.z /= value;
    return v;
}

Vector reflect(Vector a, Vector b) {
//    return minusVector(b, scaleVector(2 * dot(a, b), a));
    float x = 2 * (a.x*b.x + a.y*b.y + a.z*b.z);
    b.x -= a.x * x;
    b.y -= a.y * x;
    b.z -= a.z * x;
    return b;
}

Vector refract(Vector a, Vector b, float ior, float NdotI) {
    if (NdotI > 0.0f) {
//        b = scaleVector(-1.0f, b);
        b.x = -b.x;
        b.y = -b.y;
        b.z = -b.z;
    }
    else {
        ior = 1.0f / ior;
        NdotI = -NdotI;
    }
    float cos_t = sqrtf(1.0f - ior * ior * (1.0f - NdotI * NdotI));
    if (cos_t < 1.0f) {
//        return addVector(scaleVector(ior, a), scaleVector(ior * NdotI - cos_t, b));
        float c = ior * NdotI - cos_t;
        a.x = ior * a.x + c * b.x;
        a.y = ior * a.y + c * b.y;
        a.z = ior * a.z + c * b.z;
        return a;
    }
    else {
        return reflect(a, b);
    }
}

float quickclamp(float f) {
    return fmaxf(0.0f, fminf(f, 1.0f));
}

Vector quickclampVector(Vector v) {
//    v.x = quickclamp(v.x);
//    v.y = quickclamp(v.y);
//    v.z = quickclamp(v.z);
    v.x = fmaxf(0.0f, fminf(v.x, 1.0f));
    v.y = fmaxf(0.0f, fminf(v.y, 1.0f));
    v.z = fmaxf(0.0f, fminf(v.z, 1.0f));
    return v;
}

float averageVector(Vector v) {
    return (v.x + v.y + v.z)/3.0f;
}

#endif /* Vector_h */
