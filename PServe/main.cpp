//
//  main.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/26/20.
//

#include <string>
#include <iostream>
#include "Render.h"
#include <OpenImageDenoise/oidn.hpp>


int main(int argc, char** argv){
    if (argc < 2) {
        std::cout << "enter an 'out' fileName" << std::endl;
        return -1;
    }

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 2560;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples = 10;
    const int max_depth = 50;
   

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
    
    oidn::DeviceRef device = oidn::newDevice();
    device.commit();
    
    oidn::FilterRef filter = device.newFilter("RT");


    FrameBuffer* frame = new FrameBuffer(image_width, image_height);

    float* outColor = (float*)malloc(image_width * image_height * 3 * sizeof(float));
    uint8_t* image = (uint8_t*)malloc(image_width * image_height * 3 * sizeof(uint8_t));

    for(int frameNum = 0; frameNum < frameCount; frameNum++){
        std::cout << "rendering" << std::endl;
        render(image_width, image_height, samples, max_depth, cam, bvh, frame);


        std::cout << "denoising" << std::endl;

        // Create a filter for denoising a beauty (color) image using prefiltered auxiliary images too
        filter.setImage("color", frame->color, oidn::Format::Float3, image_width, image_height); // beauty
        filter.setImage("albedo", frame->albedo, oidn::Format::Float3, image_width, image_height); // auxiliary
        filter.setImage("normal", frame->normal, oidn::Format::Float3, image_width, image_height); // auxiliary
        filter.setImage("output", outColor, oidn::Format::Float3, image_width, image_height); // denoised beauty
        filter.set("hdr", true); // beauty image is HDR
        filter.set("cleanAux", true); // auxiliary images will be prefiltered
        filter.commit();

        // Create a separate filter for denoising an auxiliary albedo image (in-place)
        oidn::FilterRef albedoFilter = device.newFilter("RT"); // same filter type as for beauty
        albedoFilter.setImage("albedo", frame->albedo, oidn::Format::Float3, image_width, image_height);
        albedoFilter.setImage("output", frame->albedo, oidn::Format::Float3, image_width, image_height);
        albedoFilter.commit();

        // Create a separate filter for denoising an auxiliary normal image (in-place)
        oidn::FilterRef normalFilter = device.newFilter("RT"); // same filter type as for beauty
        normalFilter.setImage("normal", frame->normal, oidn::Format::Float3, image_width, image_height);
        normalFilter.setImage("output", frame->normal, oidn::Format::Float3, image_width, image_height);
        normalFilter.commit();

        // Prefilter the auxiliary images
        albedoFilter.execute();
        normalFilter.execute();

        // Filter the beauty image
        filter.execute();
        
        const char* errorMessage;
        if (device.getError(errorMessage) != oidn::Error::None)
            std::cout << "Error: " << errorMessage << std::endl;


        std::cout << "writing images" << std::endl;

        //albedo
        imageToColor(image_width, image_height, image, frame->albedo);
        saveImage("", std::string(argv[1]) + "-albedo", image_width, image_height, image);

        //normal
        imageToColor(image_width, image_height, image, frame->normal, true);
        saveImage("", std::string(argv[1]) + "-normal", image_width, image_height, image);

        //noisy
        imageToColor(image_width, image_height, image, frame->color);
        saveImage("", std::string(argv[1]) + "-noisy", image_width, image_height, image);


        //denoised
        imageToColor(image_width, image_height, image, outColor);
        saveImage("", std::string(argv[1]), image_width, image_height, image);
    }
    
    free(outColor);
    free(image);
    delete frame;

    return 0;
}


