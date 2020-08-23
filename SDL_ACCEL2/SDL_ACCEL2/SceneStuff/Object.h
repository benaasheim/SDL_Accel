//
//  Object.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
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
    Vector z; //third vertex of triangle
    Hit* (*interesect)(Ray* ray, struct Object* obj); //intersection method
    int mat; //photo-material reference in scene materials-list
} Object;

Hit* calcIntersectionPlane(Ray* ray, struct Object* plane) {
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

Object makeObject(Vector center, Vector p2, Vector p3, Hit* (*inter)(Ray* ray, struct Object* obj), int material) {
    Object object;
    object.c = center;
    object.n = p2;
    object.z = p3;
    object.interesect = inter;
    object.mat = material;
    return object;
}

Object makeSphere(Vector center, float radius, int material) {
    Object object;
    Vector oth = newVector(radius, 0, 0);
    Hit* (*inter)(Ray* ray, struct Object* obj) = calcIntersectionSphere;
    object = makeObject(center, oth, oth, inter, material);
    return object;
}

Object makePlane(Vector center, Vector radius, int material) {
    Object object;
    Hit* (*inter)(Ray* ray, struct Object* obj) = calcIntersectionPlane;
    object = makeObject(center, radius, newVector(0, 0, 0), inter, material);
    return object;
}

Object makeTriangle(Vector center, Vector radius, Vector third, int material) {
    Object object;
    Hit* (*inter)(Ray* ray, struct Object* obj) = calcIntersectionPlane;
    object = makeObject(center, radius, third, inter, material);
    return object;
}

#endif /* Object_h */
