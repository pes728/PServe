//
//  main.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/26/20.
//

#include <string>
#include <iostream>
#include "Window.h"
#include <OpenImageDenoise/oidn.hpp>



int main(int argc, char** argv){
    if (argc < 2) {
        std::cout << "enter an 'out' fileName" << std::endl;
        return -1;
    }

    auto aspect_ratio = 16.0 / 9.0;
    unsigned int image_width = 2560;
    unsigned int image_height = static_cast<int>(image_width / aspect_ratio);
    unsigned int samples = 10;
    unsigned int max_depth = 50;
   
    
    hittable_list w = random_scene();

    bvh_node bvh(w, 0, 1);

    // Camera
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0, 1.0);
   
    int frameCount = 1;

    //framebuffer
    FrameBuffer* frame = new FrameBuffer(image_width, image_height);

    RenderSettings settings(image_width, image_height, samples, max_depth, cam, &bvh, frame);

    Window window(settings, argv[1]);
    if (window.createWindow()) {
        std::cout << "Failed To Initialize Window" << std::endl;
        return -1;
    }
    window.begin();


    return 0;
}


