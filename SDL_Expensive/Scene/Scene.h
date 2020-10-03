//
//  Scene.h
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include "Light.h"
#include "Configuration.h"
#include "Material.h"
#include "Errors.h"

#include "Triangles.h"
#include "Spheres.h"
#include "Planes.h"

// Scene information
extern unsigned int numLights;
extern Light* lights;
extern unsigned int numMaterials;
extern Material* materials;

void init_Scene(void);

void update_Scene(float dt);

#endif /* Scene_h */
