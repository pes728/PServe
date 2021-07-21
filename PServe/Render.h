//
//  Render.hpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#ifndef Render_h
#define Render_h
#include <thread>
#include "Camera.h"
#include "World.h"
#include "common.h"
#include "Image.h"
#include <iostream>
#include "Sphere.h"
#include "moving_sphere.h"
#include "Material.h"
#include <ctime>
#include <iomanip>

void render(int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, uint8_t* data);

World random_scene();

World one_sphere();

void render_pixel(int x, int y, int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, uint8_t* data);

vec3 ray_colorR(const ray& r, const hittable& world, int max_depth);

#endif /* Render_hpp */
