//
//  raytrace.h
//  SDL_Accel
//
//  Created by Ben on 7/19/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef raytrace_h
#define raytrace_h
#include <math.h>
#include "Hit.h"
#include "RGBf.h"
#include "Settings.h"
#include "Materials.h"
#include "Ray.h"

typedef struct {
    Vector c;
    Vector n;
    Hit* (*interesect)(Ray, void* obj);
    Material* mat;
    int id;
} Object;

Hit* calcIntersectionPlane(Ray* ray, void* p) {
    Object* sphere = (Object*)p;
    Vector SP;
    float t, b, d;
    
    SP = minusVector(sphere->c, ray->origin);
    b = dot(SP, ray->direction);
    d = b * b - dot(SP, SP) + sphere->n.x * sphere->n.x;
    
    if (d < 0) {
        return NULL;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon() ? t : ((t = b + d) > epsilon() ? t : -1.0f);
    
    if (t == -1.0f) {
        return NULL;
    }
    
    Vector pos = addVector(ray->origin, scaleVector(t, ray->direction));
    Vector norm = divideFloat(sphere->n.x, minusVector(pos, sphere->c));
    Hit hit;
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = sphere;
    return &hit;
}

Hit* calcIntersectionSphere(Ray* ray, void* p) {
    Object* sphere = (Object*)p;
    Vector SP;
    float t, b, d;
    
    SP = minusVector(sphere->c, ray->origin);
    b = dot(SP, ray->direction);
    d = b * b - dot(SP, SP) + sphere->n.x * sphere->n.x;
    
    if (d < 0) {
        return NULL;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon() ? t : ((t = b + d) > epsilon() ? t : -1.0f);
    
    if (t == -1.0f) {
        return NULL;
    }
    
    Vector pos = addVector(ray->origin, scaleVector(t, ray->direction));
    Vector norm = divideFloat(sphere->n.x, minusVector(pos, sphere->c));
    Hit hit;
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = sphere;
    return &hit;
}

Object makeObject(Vector center, Vector p2, Hit* (*inter)(Ray, void* obj), int i) {
    Object object;
    object.c = center;
    object.n = p2;
    object.interesect = inter;
    object.id = i;
    return object;
}

Object makeSphere(Vector center, float radius, Vector col, int i, float rr, int reflec) {
    Object object;
    Vector oth = newVector(radius, 0, 0);
    Hit* (*inter)(Ray, void* obj) = calcIntersectionSphere;
    object = makeObject(center, oth, inter, i);
    return object;
}

Object makePlane(Vector center, Vector radius, Vector col, int i, float rr, int reflec) {
    Object object;
    Hit* (*inter)(Ray, void* obj) = calcIntersectionPlane;
    object = makeObject(center, radius, inter, i);
    return object;
}



#endif /* raytrace_h */
