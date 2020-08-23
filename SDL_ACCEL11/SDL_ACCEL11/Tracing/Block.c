////
////  Block.c
////  SDL_ACCEL11
////
////  Created by Ben on 8/18/20.
////  Copyright © 2020 Ben. All rights reserved.
////
//
//#include "Block.h"
//simd_float3 right, up, forward;
//
//int get_New_Blocks(Block block) {
////        printf("HERE");
//    return 2;
//}
//
//void drawBlocks(Camera* camera, SDL_Renderer* renderer, Scene* scene) {
//    
//    right = camera_Right(camera);
//
//    up = camera_Up(camera);
//
//    forward = camera_Forward(camera);
//    
//    Block block;
//    
//    block.rect.x = 0;
//    block.rect.y = 0;
//    block.rect.h = window_height;
//    block.rect.w = window_width;
//    
//    block.dgd = 0;
//    
//    block.ray.origin = camera->position;
//    
//    drawBlock(block, renderer, scene);
//    return;
//}
//
//void degradeBlock(Block block, SDL_Renderer* renderer, Scene* scene) {
////    printf("HERE");
//    int newblocks = get_New_Blocks(block);
//    
//    int new_width = block.rect.w/newblocks;
//    int new_height = block.rect.h/newblocks;
//    
//    Block next;
//    
//    next.rect.h = new_height;
//    next.rect.w = new_width;
//    next.dgd = 0;
//    
//    for (int x = 0; x < newblocks * new_width; x+=new_width) {
//        for (int y = 0; y < newblocks * new_height; y+=new_height) {
//            if (x != 0 || y != 0) {
//                next.rect.x = block.rect.x + x;
//                next.rect.y = block.rect.y + y;
//                
//                drawBlock(next, renderer, scene);
//            }
//        }
//    }
//    return;
//}
//
//void drawBlock(Block block, SDL_Renderer* renderer, Scene* scene) {
//    
//    block.ray.direction = calcCameraRayDirection(right, up, forward, block.rect.y * window_width + block.rect.x);
//    block.color = traceColor(block.ray, scene, 0, &block.dgd) * 255;
//    
//    //draw original
////    SDL_SetRenderDrawColor(renderer, block.color.x, block.color.y, block.color.z, 255);
////
////    SDL_RenderFillRect(renderer, &block.rect);
//    renderHelp(block);
//    
//    if (block.rect.h > 1 && block.rect.w > 1
////        && block.dgd
//        ) {
//        degradeBlock(block, renderer, scene);
//    }
//    
//    return;
//}
