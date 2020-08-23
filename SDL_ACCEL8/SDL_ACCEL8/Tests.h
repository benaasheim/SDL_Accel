//
//  Header.h
//  SDL_ACCEL8
//
//  Created by Ben on 8/11/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include "Vector.h"

void runTests() {
    vec a = {{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}};
    vec b = {{1, 1, 3, 4}, {1, 2, 1, 4}, {1, 2, 3, 1}};
    vec c = {{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}};
    addVec(&a, &b, &c);
    printVec(c);
}

#endif /* Header_h */
