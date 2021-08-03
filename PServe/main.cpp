//
//  main.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/26/20.
//

#include <string>
#include <iostream>
#include "Render.h"
//#include <OpenImageDenoise/oidn.hpp>

#define OVERWRITE 1

int main(int argc, char** argv){
    if (argc < 2) {
        std::cout << "enter an 'out' fileName" << std::endl;
        return -1;
    }


    if (OVERWRITE) {
        if (FILE* file = fopen(argv[1], "r")) {
            fclose(file);
            if (remove(argv[1]) != 0) {
                std::cerr << "Error deleteing file" << std::endl;
            }
        }
    }


    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 2560;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples = 10;
    const int max_depth = 50;
    
    //oidn::DeviceRef device = oidn::newDevice();
    //device.commit();

    hittable_list w = one_sphere();

    bvh_node bvh(w, 0, 1);

    // Camera
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0, 1.0);
    
    int frameCount = 1;
    
    FrameBuffer* frame = new FrameBuffer(image_width, image_height);

    //float* outData = (float*)malloc(image_width * image_height * 3 * sizeof(float));
    uint8_t* image = (uint8_t*)malloc(image_width * image_height * 3 * sizeof(uint8_t));
    for(int frameNum = 0; frameNum < frameCount; frameNum++){
        //init(image_width, image_height, frame->color, vec3(0,0,0));
        
        std::cout << "rendering" << std::endl;
        render(image_width, image_height, samples, max_depth, cam, bvh, frame);
        std::cout << "writing to image" << std::endl;
        imageToColor(image_width, image_height, image, frame->color);

        saveImage("", std::string(argv[1]), image_width, image_height, image);
    }
    
    free(image);
    delete frame;

    return 0;
}


