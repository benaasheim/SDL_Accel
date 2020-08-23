//
//  Scene.h
//  SDL_Accel
//
//  Created by Ben on 7/22/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#define max_lights 50
#define max_objects 100
#define max_materials 50

#include "Object.h"
#include "Light.h"

typedef struct {
    // Scene information
    unsigned int numObjects;
    Object objects[max_objects];
    unsigned int numLights;
    Light lights[max_lights];
    unsigned int numMaterials;
    Material materials[max_materials];
} Scene;

Scene newScene(void (*initmode)(Scene* scene)) {
    Scene scene;
    scene.numObjects = 0;
    scene.numLights = 0;
    scene.numMaterials = 0;
    initmode(&scene);
    return scene;
}

void add_Light(Scene* scene, Light light) {
    if (scene->numLights < max_lights) {
        scene->lights[scene->numLights++] = light;
    }
}

void add_Object(Scene* scene, Object object) {
    if (scene->numObjects < max_objects) {
        scene->objects[scene->numObjects++] = object;
    }
}

void add_Material(Scene* scene, Material material) {
    if (scene->numMaterials < max_materials) {
        scene->materials[scene->numMaterials++] = material;
    }
}

Material* diffWhite(Scene* scene) {
    return &scene->materials[0];
}

Material* diffRed(Scene* scene) {
    return &scene->materials[1];
}

Material* diffBlue(Scene* scene) {
    return &scene->materials[2];
}

Material* reflMetal(Scene* scene) {
    return &scene->materials[3];
}
Material* refrGlass(Scene* scene) {
    return &scene->materials[4];
}

void init_Scene1(Scene* scene) {
    //Lights
    add_Light(scene, newLight(newVector(1.0f, 1.0f, 1.0f), newVector(0.0f, 4.0f, -5.0f), 1.0f, 0.0f, 0.0f, 0.1f));
    
    //Materials
    add_Material(scene, Diffuse_White());
    add_Material(scene, Diffuse_Red());
    add_Material(scene, Diffuse_Blue());
    add_Material(scene, Reflective_Metal());
    add_Material(scene, Refractive_Glass());
    
    //Objects
    add_Object(scene, makeSphere(newVector(2, 0.75, -2), 0.75, 3));
    add_Object(scene, makeSphere(newVector(0, 0.75, -3), 0.25, 4));
    add_Object(scene, makeSphere(newVector(-2, 0.75, -4), 0.25, 4));
    add_Object(scene, makePlane(newVector(0, 0, 0), newVector(0, 1, 0), 0));
    add_Object(scene, makePlane(newVector(0, 5, 0), newVector(0, -1, 0), 0));
    add_Object(scene, makePlane(newVector(0, 0, -10), newVector(0, 0, 1), 0));
    add_Object(scene, makePlane(newVector(4, 0, 0), newVector(-1, 0, 0), 1));
    add_Object(scene, makePlane(newVector(-4, 0, 0), newVector(1, 0, 0), 2));
}

void update_Scene(Scene* scene, float dt) {
    
}

#endif /* Scene_h */
