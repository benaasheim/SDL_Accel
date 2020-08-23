//
//  RGBf.h
//  SDL_Accel
//
//  Created by Ben on 7/21/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef RGBf_h
#define RGBf_h
#include <math.h>
#include "Hit.h"

typedef struct {
    float r;
    float g;
    float b;
} RGBf;

// Create an RGBf with the given values
RGBf newRGB(float red, float green, float blue) {
    RGBf result;
    result.r = red;
    result.g = green;
    result.b = blue;
    return result;
}

// Scale a color by a constant float value
RGBf scaleRGB(RGBf color, float value) {
    return newRGB(color.r * value, color.g * value, color.b * value);
}

// Add two colors together
RGBf addRGB(RGBf a, RGBf b) {
    return newRGB(a.r + b.r, a.g + b.g, a.b + b.b);
}

void setPixelColor(RGBf pixelColor, RGBf* pixel) {
    pixel->r = pixelColor.r / 255;
    pixel->g = pixelColor.g / 255;
    pixel->b = pixelColor.b / 255;
}

//RGBf shade(Hit hit, Ray ray, int recur);

#endif /* RGBf_h */
