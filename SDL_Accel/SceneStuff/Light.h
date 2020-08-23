//
//  Light.h
//  SDL_Accel
//
//  Created by Ben on 7/21/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Light_h
#define Light_h


typedef struct {
    Vector position;
    Vector color;
    float
//    intensity
//    ,
    constant
    , linear
    , exponent
    ;
} Light;

Light newLight(Vector color, Vector position, float intensity, float constant, float linear, float exponent) {
    Light light;
    light.position = position;
    light.color = color;
//    light.intensity = intensity;
    light.constant = constant;
    light.linear = linear;
    light.exponent = exponent;
    return light;
}

Light newLight2(Vector color, Vector position, float intensity, float constant, float linear, float exponent) {
    Light light;
    light.position = position;
    light.color = scaleVector(intensity, color);
//    light.intensity = intensity;
    light.constant = constant;
    light.linear = linear;
    light.exponent = exponent;
    return light;
}


#endif /* Light_h */
