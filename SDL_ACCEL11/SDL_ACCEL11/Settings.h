//
//  Settings.h
//  SDL_ACCEL11
//
//  Created by Ben on 8/18/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Settings_h
#define Settings_h

// Control variables for the different features
#define window_width 729
#define window_height 729
#define aspect_ratio (window_width / window_height)
#define total_pixels (window_height * window_width)
#define resolution 1
#define dheight (window_height * resolution)
#define dwidth (window_width * resolution)
#define epsilon 1e-3f
#define tthreads 8
#define threads (tthreads * tthreads)
#define max_recursion 5
#define camera_speed 0.5f
#define camera_sensitivity 0.5f
#define step_placeholder (total_pixels / threads);
#define y_placeholder (window_height / threads);
#define dgd_neutral 1
#define dgd_accel 3

#endif /* Settings_h */
