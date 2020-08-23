//
//  Object.h
//  SDL_ACCEL3
//
//  Created by Ben on 7/28/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Object_h
#define Object_h
#include <math.h>
#include "Hit.h"
#include "Settings.h"
#include "Materials.h"
#include "Ray.h"

#define squareff(f) (f * f)

typedef struct Object {
    Vector c; //center of Circle, center/position of Plane
    Vector n; //xCoord is radius of Circle, normal of Plane
    Vector z; //third vertex of triangle
    Hit* (*interesect)(Ray ray, struct Object* obj); //intersection method
    float (*gett)(Ray ray, struct Object* obj); //intersection method
    int mat; //photo-material reference in scene materials-list
} Object;

Hit* calcIntersectionPlane(Ray ray, struct Object* plane) {
    
    float d = dot(plane->n, ray.direction);
    
    if (d > 0.0f) {
        return NULL;
    }
        
    Vector P = minusVector(plane->c, ray.origin);
    float t = dot(P, plane->n)/d;
    
    if (t < epsilon) {
        return NULL;
    }
    
    Vector pos = addVector(ray.origin, scaleVector(t, ray.direction));
    Vector norm = normalize(plane->n);
    Hit hit;
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = plane;
//    printf("HIT_Plane!\n");
    return &hit;
}

float calcPlaneT(Ray ray, struct Object* plane) {
    
    float d = dot(plane->n, ray.direction);
    
    if (d > 0.0f) {
        return -1.0f;
    }
        
    Vector P = minusVector(plane->c, ray.origin);
    float t = dot(P, plane->n)/d;
    
    if (t < epsilon) {
        return -1.0f;
    }
    else {
        return t;
    }
}

Hit* calcIntersectionSphere(Ray ray, struct Object* sphere) {
    Vector SP;
    float t, b, d;
    
    SP = minusVector(sphere->c, ray.origin);
    b = dot(SP, ray.direction);
    d = b * b - dot(SP, SP) +
//    sphere->n.x * sphere->n.x
    squareff(sphere->n.x);
    ;
    
    if (d < 0.0f) {
        return NULL;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    
    if (t == -1.0f) {
        return NULL;
    }
    
    Vector pos = addVector(ray.origin, scaleVector(t, ray.direction));
    Vector norm = divideFloat(sphere->n.x, minusVector(pos, sphere->c));
    Hit hit;
    hit.position = pos;
    hit.normal = norm;
    hit.t = t;
    hit.obj = sphere;
//    printf("HIT_SPHERE!");
    return &hit;
}

float calcSphereT(Ray ray, struct Object* sphere) {
    Vector SP;
    float t, b, d;
    
    SP = minusVector(sphere->c, ray.origin);
    b = dot(SP, ray.direction);
    d = b * b - dot(SP, SP) +
//    sphere->n.x * sphere->n.x
    squareff(sphere->n.x);
    ;
    
    if (d < 0.0f) {
        return -1.0f;
    }
    
    d = sqrtf(d);
    
    t = (t = b - d) > epsilon ? t : ((t = b + d) > epsilon ? t : -1.0f);
    
    if (t == -1.0f) {
        return -1.0f;
    }
    else {
        return t;
    }
}

Object makeObject(Vector center, Vector p2, Vector p3, Hit* (*inter)(Ray ray, struct Object* obj), float (*gt)(Ray ray, struct Object* obj), int material) {
    Object object;
    object.c = center;
    object.n = p2;
    object.z = p3;
    object.interesect = inter;
    object.gett = gt;
    object.mat = material;
    return object;
}

Object makeSphere(Vector center, float radius, int material) {
    Object object;
    Vector oth = newVector(radius, 0, 0);
    Hit* (*inter)(Ray ray, struct Object* obj) = calcIntersectionSphere;
    float (*gt)(Ray ray, struct Object* obj) = calcSphereT;
    object = makeObject(center, oth, oth, inter, gt, material);
    return object;
}

Object makePlane(Vector center, Vector radius, int material) {
    Object object;
    Hit* (*inter)(Ray ray, struct Object* obj) = calcIntersectionPlane;
    float (*gt)(Ray ray, struct Object* obj) = calcPlaneT;
    object = makeObject(center, radius, newVector(0, 0, 0), inter, gt, material);
    return object;
}

//Object makeTriangle(Vector center, Vector radius, Vector third, int material) {
//    Object object;
//    Hit* (*inter)(Ray ray, struct Object* obj) = calcIntersectionPlane;
//    object = makeObject(center, radius, third, inter, material);
//    return object;
//}


#endif /* Object_h */
