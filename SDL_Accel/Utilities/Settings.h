//
//  Settings.h
//  SDL_Accel
//
//  Created by Ben on 7/21/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Settings_h
#define Settings_h

// Control variables for the different features
#define epsilon 1e-3f
#define threads 4
#define max_recursion 5
#define camera_speed 0.5
#define camera_sensitivity 10
#define total_pixels (window_height * window_width)
#define step_placeholder (total_pixels / threads);


#endif /* Settings_h */
