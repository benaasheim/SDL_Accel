//
//  Scene.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#define max_lights 50
#define max_objects 100
#define max_materials 50

#include "Objects.h"
#include "Light.h"
#include "Materials.h"
#include "Sun.h"

struct __attribute__ ((__packed__)) Scene {
    // Scene information
    unsigned int numObjects;
    Object objects[max_objects];
    unsigned int numLights;
    Light lights[max_lights];
    unsigned int numMaterials;
    Material materials[max_materials];
};

typedef struct Scene Scene;

Scene newScene(void (*initmode)(Scene* scene));

void add_Light(Scene* scene, Light light);

void add_Object(Scene* scene, Object object);

void add_Material(Scene* scene, Material material);

void add_Sun(Scene* scene, Sun sun);

Material* diffWhite(Scene* scene);

Material* diffRed(Scene* scene);

Material* diffBlue(Scene* scene);

Material* reflMetal(Scene* scene);
Material* refrGlass(Scene* scene);

void init_Scene1(Scene* scene);

void init_Scene2(Scene* scene);

void init_Scene3(Scene* scene);

void update_Scene(Scene* scene, float dt);

#endif /* Scene_h */
