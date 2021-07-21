//
//  Render.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#include "Render.h"


World one_sphere(){
    World world;

    auto ground_material = make_shared<lambertian>(vec3(1.0, 0.0, 0.0));
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, ground_material));
    
    auto material1 = make_shared<metal>(vec3(0.7, 0.3, 0.2), 0.1);
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));
    
    return world;
}

World random_scene() {
    World world;

    auto ground_material = make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = vec3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

    return world;
}

vec3 ray_colorR(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return vec3(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        vec3 attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_colorR(scattered, world, depth-1);
        return vec3(0,0,0);
    }

    
    return vec3(1,1,1);
    //vec3 unit_direction = unit_vector(r.direction());
    //auto t = 0.5*(unit_direction.y() + 1.0);
    //return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

void render_pixel(int x, int y, int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, uint8_t* data){
    vec3 pixel(0,0,0);
    for (int s = 0; s < samples; s++) {
        auto u = (x + random_double()) / (image_width-1);
        auto v = (y + random_double()) / (image_height-1);
        ray r = c.get_ray(u, v);
        pixel += ray_colorR(r, world, max_depth);
    }
    setPixel(x, image_height - 1 - y, image_width, Color(pixel, samples), data);
}

void thread_manager(int id, int jump_value, int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, uint8_t* data){
    int index = id;
    while(index < image_width * image_height){
        render_pixel(index % image_width , index / image_width, image_width, image_height, samples, max_depth, c, world, data);
        index += jump_value;
    }
    std::cout << "Thread " << id << ": DONE" << std::endl;
}

void render(int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, uint8_t* data){
    const auto numThreads = std::thread::hardware_concurrency();
    std::thread t[numThreads];

    std::cout << "Beginning\nTotal threads: " << numThreads << std::endl;
    
    std::clock_t start = std::clock();
    
    for(int i = 0; i < numThreads; i++) t[i] = std::thread(thread_manager, i, numThreads, image_width, image_height, samples, max_depth, c, std::ref(world), data);
    
    for(int i = 0; i < numThreads; i++) t[i].join();
    std::cout << "ALL DONE!" << std::endl;
    std::clock_t end = std::clock();
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
    << (end-start) / CLOCKS_PER_SEC << " s\n";
}
