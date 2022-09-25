//
//  Render.hpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#ifndef Render_h
#define Render_h

#include <thread>
#include "bvh_node.h"
#include "hittable_list.h"
#include "common.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "moving_sphere.h"
#include "Material.h"
#include <ctime>
#include "FrameBuffer.h"
#include <iomanip>

//#include <OpenImageDenoise/oidn.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

const int MULTI_THREADED = 1;

class Camera {
    public:
        Camera() {}
        Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect_ratio, float aperture, float focus_dist, float t0, float t1);

        ray get_ray(float s, float t) const {
            glm::vec3 rd = lens_radius * random_in_unit_disk();
            glm::vec3 offset = u * rd.x + v * rd.y;
            return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, randomFloat(time0, time1));
        }

        glm::vec3 origin;
        glm::vec3 lower_left_corner;
        glm::vec3 horizontal;
        glm::vec3 vertical;
        glm::vec3 u, v, w;
        float lens_radius;
        float time0, time1;

        //intital variables
        glm::vec3 lookat, vup;
        float vfov, aperture, focus_dist;
    };

	

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
    void toFile(const char* file) const;

    unsigned int width, height;
    unsigned int samples;
    unsigned int max_depth;
    Camera cam;
    hittable* world;
    FrameBuffer* framebuffer;
    float* beauty;
};
    

int getThreadCount();

void render(RenderSettings settings, std::atomic_bool* finished);

hittable_list random_scene();

hittable_list one_sphere();

void render_pixel(int x, int y, int image_width, int image_height, int samples, int max_depth, Camera c, hittable* world, FrameBuffer* frame);

Frame ray_colorR(const ray& r, hittable* world, int max_depth);

//void denoise(RenderSettings settings, std::atomic_bool* finsihedDenoising);

//Frame ray_colorI(const ray& r, const hittable& world, int max_depth);

#endif
