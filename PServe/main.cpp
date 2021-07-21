//
//  main.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/26/20.
//
#include <string>
#include <iostream>
#include "Render.h"


int main(int argc, char** argv){
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 100;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples = 1;
    const int max_depth = 50;
    
    
    World w = random_scene();


    // Camera
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0, 1.0);
    
    int frameCount = 1;
    
    
    uint8_t* data = (uint8_t*)malloc(image_width * image_height * 3 * sizeof(uint8_t));

    for(int frame = 0; frame < frameCount; frame++){
        init(image_width, image_height, data, Color(0,0,0));
        
        render(image_width, image_height, samples, max_depth, cam, w, data);
    
        saveImage("", std::to_string(frame), image_width, image_height, data);
    }
    
    free(data);
    
    return 0;
}


