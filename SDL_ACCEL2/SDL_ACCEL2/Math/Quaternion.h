//
//  Quaternion.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Quaternion_h
#define Quaternion_h

#include "Vector.h"

typedef struct {
    float w;
    float x;
    float y;
    float z;
} Quaternion;

// Create a new Quaternion with the given values
Quaternion newQuaternion(float w, float x, float y, float z) {
    Quaternion result;
    
    result.w = w;
    result.x = x;
    result.y = y;
    result.z = z;
    
    return result;
}

// Create a new Quaternion with the given values
Quaternion newQuaternionVT(Vector v, float theta) {
    theta = theta * 3.14 / 360;
    float stheta = sinf(theta);
    
    Quaternion result;
    result.w = cosf(theta);
    result.x = v.x * stheta;
    result.y = v.y * stheta;
    result.z = v.z * stheta;
    return result;
}

Quaternion multQuat(Quaternion a, Quaternion q) {
    Quaternion r;
    
    r.w = a.w * q.w - a.x * q.x - a.y * q.y - a.z * q.z;
    r.x = a.w * q.x + a.x * q.w + a.y * q.z - a.z * q.y;
    r.y = a.w * q.y - a.x * q.z + a.y * q.w + a.z * q.x;
    r.z = a.w * q.z + a.x * q.y - a.y * q.x + a.z * q.w;
    
    return r;
}

Quaternion multVec(Quaternion a, Vector v) {
    Quaternion r;
    
    r.w = -a.x * v.x - a.y * v.y - a.z * v.z;
    r.x = a.w * v.x + a.y * v.z - a.z * v.y;
    r.y = a.w * v.y + a.z * v.x - a.x * v.z;
    r.z = a.w * v.z + a.x * v.y - a.y * v.x;
    
    return r;
}



Quaternion divideQuat(Quaternion a, float f) {
    return newQuaternion(a.w/f, a.x/f, a.y/f, a.z/f);
}

Quaternion conjugate(Quaternion a) {
    return newQuaternion(a.w, -a.x, -a.y, -a.z);
}

Vector mulByVec(Quaternion a, Vector v) {
    Quaternion q_inv = conjugate(a);
    Quaternion w = multQuat(multVec(a, v), q_inv);
    
    return newVector(w.x, w.y, w.z);
}

float qLength(Quaternion a) {
    return sqrtf(a.w*a.w+a.x*a.x+a.y*a.y+a.z*a.z);
}

Quaternion normalizeQ(Quaternion a) {
    return divideQuat(a, qLength(a));
}

Vector getForwardVector(Quaternion a) {
    return mulByVec(a, newVector(0, 0, 1));
}

Vector getUpVector(Quaternion a) {
    Vector v = newVector(0, 1, 0);
    return mulByVec(a, v);
}

Vector getRightVector(Quaternion a) {
    return mulByVec(a, newVector(1, 0, 0));
}

#endif /* Quaternion_h */
