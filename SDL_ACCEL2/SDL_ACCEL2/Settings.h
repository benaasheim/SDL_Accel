//
//  Settings.h
//  SDL_ACCEL2
//
//  Created by Ben on 7/26/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Settings_h
#define Settings_h

// Control variables for the different features
#define window_width 350
#define window_height 350
#define resolution 2
#define dheight (window_height * resolution)
#define dwidth (window_width * resolution)
#define epsilon 1e-3f
#define threads 64
#define max_recursion 4
#define camera_speed 0.5
#define camera_sensitivity 10
#define total_pixels (window_height * window_width)
#define step_placeholder (total_pixels / threads);

#endif /* Settings_h */
