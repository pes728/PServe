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

#include <OpenImageDenoise/oidn.hpp>

struct RenderSettings {
public:
	RenderSettings() {}
	RenderSettings(unsigned int width, unsigned int height, unsigned int samples, unsigned int max_depth, Camera cam, hittable* world, FrameBuffer* framebuffer) {
		this->width = width;
		this->height = height;
		this->samples = samples;
		this->max_depth = max_depth;
		this->cam = cam;
		this->world = world;
		this->framebuffer = framebuffer;
		beauty = (float*)malloc(width * height * 3 * sizeof(float));
	}

	unsigned int width, height;
	unsigned int samples;
	unsigned int max_depth;
	Camera cam;
	hittable* world;
	FrameBuffer* framebuffer;
	float* beauty;
};

void render(RenderSettings settings, std::atomic_bool* finished);

hittable_list random_scene();

hittable_list one_sphere();

void render_pixel(int x, int y, int image_width, int image_height, int samples, int max_depth, Camera c, hittable* world, FrameBuffer* frame);

Frame ray_colorR(const ray& r, hittable* world, int max_depth);

void denoise(RenderSettings settings, std::atomic_bool* finsihedDenoising);

//Frame ray_colorI(const ray& r, const hittable& world, int max_depth);

#endif
