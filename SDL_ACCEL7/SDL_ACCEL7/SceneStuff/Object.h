//
//  Object.h
//  SDL_ACCEL7
//
//  Created by Ben on 8/8/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Object_h
#define Object_h

#include <stdio.h>
#include <math.h>
#include "Hit.h"
#include "Settings.h"
#include "Materials.h"
#include "Ray.h"

#define squareff(f) (f * f)

typedef struct Object {
    vec c; //center of Circle, center/position of Plane
    vec n; //xCoord is radius of Circle, normal of Plane
    vec z; //third vertex of triangle
    Hit* (*interesect)(Ray ray, struct Object* obj, float t); //intersection method
    float (*gett)(Ray ray, struct Object* obj); //intersection method
    int mat; //photo-material reference in scene materials-list
} Object;

Hit* calcIntersectionPlane(Ray ray, struct Object* plane) {
    
    float d = dot(&plane->n, &ray.direction);
    
    if (d > 0.0f) {
        return NULL;
    }
    
    vec P;
    subVec(&plane->c, &ray.origin, &P);
    
    float t = dot(&P, &plane->n)/d;
    
    if (t < epsilon) {
        return NULL;
    }
    Hit hit;
    vec pos;
    vec temp1;
    mulfv(t, &ray.direction, &temp1);
    addVec(&ray.origin, &temp1, &pos);
    vec norm;
    normalize(&plane->n, &norm);
    
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = plane;
    return &hit;
}

float calcPlaneT(Ray ray, struct Object* plane) {
    
    float d = dot(&plane->n, &ray.direction);
    
    if (d > 0.0f) {
        return -1.0f;
    }
        
    vec P;
    subVec(&plane->c, &ray.origin, &P);
    
    float t = dot(&P, &plane->n)/d;
    
//    if (t < epsilon) {
//        return -1.0f;
//    }
//    else {
////        printf("HIT PLANE!\n");
//        return t;
//    }
    return t;
}

Hit* calcIntersectionPlane2(Ray ray, struct Object* plane, float t) {
    Hit hit;
    mulfv(t, &ray.direction, &hit.position);
    addVec(&ray.origin, &hit.position, &hit.position);
    normalize(&plane->n, &hit.normal);
    hit.obj = plane;
    return &hit;
}

Hit* calcIntersectionSphere(Ray ray, struct Object* sphere) {
    vec SP;
    float t, b, d;
    
    subVec(&sphere->c, &ray.origin, &SP);
    
    b = dot(&SP, &ray.direction);
    
    
    
    
    d = b * b - dotSelf(&SP) + squareff(sphere->n.x);
    
    
    if (d < 0.0f) {
        return NULL;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    
    if (t == -1.0f) {
        return NULL;
    }
    
    vec pos;
//    = addVector(ray.origin, scaleVector(t, ray.direction));
    vec temp;
    mulfv(t, &ray.direction, &temp);
    addVec(&ray.origin, &temp, &pos);
    vec norm;
//    = divideFloat(sphere->n.x, minusVector(pos, sphere->c));
    vec temp2;
    subVec(&pos, &sphere->c, &temp2);
    divideFloat(&temp2, sphere->n.x, &norm);
    Hit hit;
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = sphere;
//    printf("HIT_SPHERE!");
    return &hit;
}



float calcSphereT(Ray ray, struct Object* sphere) {
    vec SP;
    float t, b, d;
    
    subVec(&sphere->c, &ray.origin, &SP);
    
    b = dot(&SP, &ray.direction);
    
    
    
    
    d = b * b - dotSelf(&SP) + squareff(sphere->n.x);
    
    
    if (d < 0.0f) {
        return -1.0f;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    
//    if (t == -1.0f) {
//        return -1.0f;
//    }
//    else {
//        return t;
//    }
    return t;
}

Hit* calcIntersectionSphere2(Ray ray, struct Object* sphere, float t) {
    Hit hit;
    mulfv(t, &ray.direction, &hit.position);
    addVec(&ray.origin, &hit.position, &hit.position);
    subVec(&hit.position, &sphere->c, &hit.normal);
    divideFloat(&hit.normal, sphere->n.x, &hit.normal);
    
    hit.obj = sphere;
    return &hit;
}

Hit* calcIntersectionTriangle(Ray ray, struct Object* plane) {
    float d = dot(&plane->n, &ray.direction);
    
    if (d > 0.0f) {
        return NULL;
    }
    
    vec P;
    subVec(&plane->c, &ray.origin, &P);
    
    float t = dot(&P, &plane->n)/d;
    
    if (t < epsilon) {
        return NULL;
    }
    
    
    
    
    
    
    
    Hit hit;
    vec pos;
    vec temp1;
    mulfv(t, &ray.direction, &temp1);
    addVec(&ray.origin, &temp1, &pos);
    vec norm;
    normalize(&plane->n, &norm);
    
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = plane;
    
    //---Triangle add
    vec p1 = hit.position;
    vec v0 = plane->c;
    vec v1 = plane->n;
    vec v2 = plane->z;
    
    vec u, v, w;
    subVec(&v1, &v0, &u);
    subVec(&v2, &v0, &v);
    subVec(&p1, &v0, &w);
    
    float uDv = dot(&u, &v);
    float wDv = dot(&w, &v);
    float vDv = dot(&v, &v);
    float wDu = dot(&w, &u);
    float uDu = dot(&u, &u);
    
    float s1 = (uDv*wDv - vDv*wDu)/(uDv*uDv - uDu*vDv);
    float t1 = (uDv*wDu - uDu*wDv)/(uDv*uDv - uDu*vDv);
    
    if (s1 >= 0.0f && t1 >= 0.0f && s1 + t1 < 1.0f) {
        return &hit;
    }
    return NULL;
}



float calcTriangleT(Ray ray, struct Object* sphere) {
    vec SP;
    float t, b, d;
    
    subVec(&sphere->c, &ray.origin, &SP);
    
    b = dot(&SP, &ray.direction);
    
    
    
    
    d = b * b - dotSelf(&SP) + squareff(sphere->n.x);
    
    
    if (d < 0.0f) {
        return -1.0f;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    
//    if (t == -1.0f) {
//        return -1.0f;
//    }
//    else {
//        return t;
//    }
    return t;
}

Hit* calcIntersectionTriangle2(Ray ray, struct Object* sphere, float t) {
    Hit hit;
    mulfv(t, &ray.direction, &hit.position);
    addVec(&ray.origin, &hit.position, &hit.position);
    subVec(&hit.position, &sphere->c, &hit.normal);
    divideFloat(&hit.normal, sphere->n.x, &hit.normal);
    
    hit.obj = sphere;
    return &hit;
}

Object makeObject(vec center, vec p2, vec p3, Hit* (*inter)(Ray ray, struct Object* obj, float t), float (*gt)(Ray ray, struct Object* obj), int material) {
    Object object;
    object.c = center;
    object.n = p2;
    object.z = p3;
    object.interesect = inter;
    object.gett = gt;
    object.mat = material;
    return object;
}

Object makeSphere(vec center, float radius, int material) {
    Object object;
    vec oth = {radius, 0, 0};
    Hit* (*inter)(Ray ray, struct Object* obj, float t) = calcIntersectionSphere2;
    float (*gt)(Ray ray, struct Object* obj) = calcSphereT;
    object = makeObject(center, oth, oth, inter, gt, material);
    return object;
}

Object makePlane(vec center, vec radius, int material) {
    Object object;
    Hit* (*inter)(Ray ray, struct Object* obj, float t) = calcIntersectionPlane2;
    float (*gt)(Ray ray, struct Object* obj) = calcPlaneT;
    vec oth = {0, 0, 0};
    object = makeObject(center, radius, oth, inter, gt, material);
    return object;
}

Object makeTriangle(vec a, vec b, vec c, int material) {
    Object object;
    
    return object;
}

#endif /* Object_h */
