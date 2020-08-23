//
//  Quaternion.h
//  SDL_ACCEL3
//
//  Created by Ben on 7/28/20.
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
//    return newQuaternion(a.w/f, a.x/f, a.y/f, a.z/f);
    a.w /= f;
    a.x /= f;
    a.y /= f;
    a.z /= f;
    return a;
}

Quaternion conjugate(Quaternion a) {
//    return newQuaternion(a.w, -a.x, -a.y, -a.z);
    a.x = -a.x;
    a.y = -a.y;
    a.z = -a.z;
    return a;
}

Vector mulByVec(Quaternion a, Vector v) {
//    Quaternion q_inv = conjugate(a);
//    Quaternion w = multQuat(multVec(a, v), q_inv);
//
////    return newVector(w.x, w.y, w.z);
//    v.x = w.x;
//    v.y = w.y;
//    v.z = w.z;
//    return v;
    
//    Quaternion q_inv;
//
//    q_inv.w = a.w;
//    q_inv.x = -a.x;
//    q_inv.y = -a.y;
//    q_inv.z = -a.z;
    
    
    Quaternion r;
    
    r.w = -a.x * v.x - a.y * v.y - a.z * v.z;
    r.x = a.w * v.x + a.y * v.z - a.z * v.y;
    r.y = a.w * v.y + a.z * v.x - a.x * v.z;
    r.z = a.w * v.z + a.x * v.y - a.y * v.x;
    
    Vector rr;
    
//    rr.w = r.w * q_inv.w - r.x * q_inv.x - r.y * q_inv.y - r.z * q_inv.z;
    rr.x = -r.w * a.x + r.x * a.w - r.y * a.z + r.z * a.y;
    rr.y = -r.w * a.y + r.x * a.z + r.y * a.w - r.z * a.x;
    rr.z = -r.w * a.z - r.x * a.y + r.y * a.x + r.z * a.w;
    
    return rr;
    
}

float qLength(Quaternion a) {
    return sqrtf(a.w*a.w+a.x*a.x+a.y*a.y+a.z*a.z);
}

Quaternion normalizeQ(Quaternion a) {
//    return divideQuat(a, qLength(a));
    float f = sqrtf(a.w*a.w+a.x*a.x+a.y*a.y+a.z*a.z);
    a.w /= f;
    a.x /= f;
    a.y /= f;
    a.z /= f;
    return a;
}

Vector getForwardVector(Quaternion a) {
//    return mulByVec(a, newVector(0, 0, 1));
    Vector rr;
    
    rr.x = 2.0f*a.z*a.x + 2.0f*a.y*a.w;
    rr.y = 2.0f*a.z*a.y - 2.0f*a.x*a.w;
    rr.z = a.z*a.z - a.y*a.y - a.x*a.x + a.w*a.w;
    
    return rr;
}

Vector getUpVector(Quaternion a) {
//    return mulByVec(a, newVector(0, 1, 0));
    Vector rr;
        
    rr.x = 2.0f*a.y*a.x - 2.0f*a.z*a.w;
    rr.y = a.y*a.y - a.z*a.z + a.w*a.w - a.x*a.x;
    rr.z = 2.0f*a.y*a.z + 2.0f*a.x*a.w;
    
    return rr;
}

Vector getRightVector(Quaternion a) {
//    return mulByVec(a, newVector(1, 0, 0));
    Vector rr;

    rr.x = a.x*a.x + a.w*a.w - a.z*a.z - a.y*a.y;
    rr.y = 2.0f*a.x*a.y + 2.0f*a.w*a.z;
    rr.z = 2.0f*a.x*a.z - 2.0f*a.w*a.y;

    return rr;
}

#endif /* Quaternion_h */
