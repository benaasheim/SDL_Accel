//
//  Tracing.h
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Tracing_h
#define Tracing_h

#include <stdio.h>
#include <simd/simd.h>

#include "Settings.h"
#include "Material.h"
#include "Configuration.h"

#include "Scene.h"

simd_float3 traceColor2(const Ray ray, const int n);

#endif /* Tracing_h */
