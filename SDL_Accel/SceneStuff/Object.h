//
//  Object.h
//  SDL_Accel
//
//  Created by Ben on 7/23/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Object_h
#define Object_h
#include <math.h>
#include "Hit.h"
#include "Settings.h"
#include "Materials.h"
#include "Ray.h"

typedef struct Object {
    Vector c; //center of Circle, center/position of Plane
    Vector n; //xCoord is radius of Circle, normal of Plane
    Hit* (*interesect)(Ray* ray, struct Object* obj); //intersection method
//    Material* mat; //photo-material
    int mat;
} Object;

Hit* calcIntersectionPlane(Ray* ray, struct Object* plane) {
//    Object* plane = (Object*)p;

    Vector P = minusVector(plane->c, ray->origin);
    float d = dot(plane->n, ray->direction);
    
    if (d > 0) {
        return NULL;
    }
        
    float t = dot(P, plane->n)/d;
    
    if (t < epsilon) {
        return NULL;
    }
    
    Vector pos = addVector(ray->origin, scaleVector(t, ray->direction));
    Vector norm = normalize(plane->n);
    Hit hit;
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = plane;
//    printf("HIT_Plane!\n");
    return &hit;
}

Hit* calcIntersectionSphere(Ray* ray, struct Object* sphere) {
//    Object* sphere = (Object*)p;
    Vector SP;
    float t, b, d;
    
    SP = minusVector(sphere->c, ray->origin);
    b = dot(SP, ray->direction);
    d = b * b - dot(SP, SP) + sphere->n.x * sphere->n.x;
    
    if (d < 0) {
        return NULL;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    
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
//    printf("HIT_SPHERE!");
    return &hit;
}

Hit* calcIntSphere3(float t, Ray* ray, Object* sphere) {
    Vector pos = addVector(ray->origin, scaleVector(t, ray->direction));
    Vector norm = divideFloat(sphere->n.x, minusVector(pos, sphere->c));
    Hit hit;
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = sphere;
//    printf("HIT_SPHERE!");
    return &hit;
}

Hit* calcIntSphere2(float d, float b, Ray* ray, Object* sphere) {
    float t;
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    
    if (t == -1.0f) {
        return NULL;
    }
    else {
        return calcIntSphere3(t, ray, sphere);
    }
}

Hit* calcIntSphere1(Ray* ray, struct Object* p) {
    Object* sphere = (Object*)p;
    Vector SP;
    float b, d;
    
    SP = minusVector(sphere->c, ray->origin);
    b = dot(SP, ray->direction);
    d = b * b - dot(SP, SP) + sphere->n.x * sphere->n.x;
    
    if (d < 0) {
        return NULL;
    }
    else {
        return calcIntSphere2(d, b, ray, sphere);
    }
}



Object makeObject(Vector center, Vector p2, Hit* (*inter)(Ray* ray, struct Object* obj), Material* material) {
    Object object;
    object.c = center;
    object.n = p2;
    object.interesect = inter;
    object.mat = material;
    return object;
}

Object makeSphere(Vector center, float radius, Material* material) {
    Object object;
    Vector oth = newVector(radius, 0, 0);
    Hit* (*inter)(Ray* ray, struct Object* obj) = calcIntersectionSphere;
    object = makeObject(center, oth, inter, material);
    return object;
}

Object makePlane(Vector center, Vector radius, Material* material) {
    Object object;
    Hit* (*inter)(Ray* ray, struct Object* obj) = calcIntersectionPlane;
    object = makeObject(center, radius, inter, material);
    return object;
}


#endif /* Object_h */
