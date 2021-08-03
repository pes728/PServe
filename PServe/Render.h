//
//  Render.hpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//



#ifndef Render_h
#define Render_h

#define MULTI_THREADED 1


#include <thread>
#include "Camera.h"
#include "bvh_node.h"
#include "hittable_list.h"
#include "common.h"
#include "Image.h"
#include <iostream>
#include "Sphere.h"
#include "moving_sphere.h"
#include "Material.h"
#include <ctime>
#include "FrameBuffer.h"
#include <iomanip>

void render(int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, FrameBuffer* frame);

hittable_list random_scene();

hittable_list one_sphere();

void render_pixel(int x, int y, int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, FrameBuffer* frame);

Frame ray_colorR(const ray& r, const hittable& world, int max_depth);

//Frame ray_colorI(const ray& r, const hittable& world, int max_depth);

#endif /* Render_hpp */
