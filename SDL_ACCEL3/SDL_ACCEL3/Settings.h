//
//  Settings.h
//  SDL_ACCEL3
//
//  Created by Ben on 7/28/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Settings_h
#define Settings_h

// Control variables for the different features
#define window_width 110
#define window_height 110
#define aspect_ratio (window_height / window_width)
#define resolution 2
#define dheight (window_height * resolution)
#define dwidth (window_width * resolution)
#define epsilon 1e-3f
#define threads 64
#define max_recursion 5
#define camera_speed 0.5
#define camera_sensitivity 5
#define total_pixels (window_height * window_width)
#define step_placeholder (total_pixels / threads);

#endif /* Settings_h */
