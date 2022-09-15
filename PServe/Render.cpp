//
//  Render.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#include "Render.h"


hittable_list one_sphere(){
    hittable_list world;

    auto checker = make_shared<checker_texture>(vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9));

    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, ground_material));
    
    auto material1 = make_shared<metal>(vec3(0.7, 0.3, 0.2), 0.1);
    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, material1));
    
    return world;
}

hittable_list random_scene() {
    hittable_list world;



    auto checker = make_shared<checker_texture>(vec3(0.2, 0.3, 0.1), vec3(0.9, 0.9, 0.9));

    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(vec3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto solid = make_shared<solid_color>(vec3(random_double(), random_double(), random_double()));
                    sphere_material = make_shared<lambertian>(solid);
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

    auto solid = make_shared<solid_color>(vec3(random_double()));
    auto material2 = make_shared<lambertian>(solid);
    world.add(make_shared<sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(vec3(4, 1, 0), 1.0, material3));

    return world;
}

vec3 Background(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

Frame ray_colorR(const ray& r, hittable* world, int depth) {
    hit_record rec;

    Frame frame;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        frame.color = vec3(0, 0, 0);
        frame.normal = vec3(0, 0, 0);
        return frame;
    }

    // If the ray hits nothing, return the background color.
    if (!world->hit(r, 0.001, infinity, rec)) {
        frame.color = frame.albedo = Background(r);
        frame.normal = unit_vector(-r.dir);
        return frame;
    }

    ray scattered;
    vec3 attenuation;
    
    if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
        frame.color = vec3(0, 0, 0);
        frame.normal = rec.normal;
        return frame;
    }

    Frame f = ray_colorR(scattered, world, depth - 1);

    frame.normal = rec.normal;
    frame.albedo = attenuation;
    frame.color = attenuation * f.color;
    return frame;
}

void denoise(RenderSettings settings, std::atomic_bool* finsihedDenoising)
{
    //setup oidn
    oidn::DeviceRef device = oidn::newDevice();
    device.commit();

    oidn::FilterRef filter = device.newFilter("RT");

    float* outColor = (float*)malloc(settings.width * settings.height * 3 * sizeof(float));
    uint8_t* image = (uint8_t*)malloc(settings.width * settings.height * 3 * sizeof(uint8_t));

    std::cout << "denoising" << std::endl;

    // Create a filter for denoising a beauty (color) image using prefiltered auxiliary images too
    filter.setImage("color", settings.framebuffer->color, oidn::Format::Float3, settings.width, settings.height); // beauty
    filter.setImage("albedo", settings.framebuffer->albedo, oidn::Format::Float3, settings.width, settings.height); // auxiliary
    filter.setImage("normal", settings.framebuffer->normal, oidn::Format::Float3, settings.width, settings.height); // auxiliary
    filter.setImage("output", outColor, oidn::Format::Float3, settings.width, settings.height); // denoised beauty
    filter.set("hdr", true); // beauty image is HDR
    filter.set("cleanAux", true); // auxiliary images will be prefiltered
    filter.commit();

    // Create a separate filter for denoising an auxiliary albedo image (in-place)
    oidn::FilterRef albedoFilter = device.newFilter("RT"); // same filter type as for beauty
    albedoFilter.setImage("albedo", settings.framebuffer->albedo, oidn::Format::Float3, settings.width, settings.height);
    albedoFilter.setImage("output", settings.framebuffer->albedo, oidn::Format::Float3, settings.width, settings.height);
    albedoFilter.commit();

    // Create a separate filter for denoising an auxiliary normal image (in-place)
    oidn::FilterRef normalFilter = device.newFilter("RT"); // same filter type as for beauty
    normalFilter.setImage("normal", settings.framebuffer->normal, oidn::Format::Float3, settings.width, settings.height);
    normalFilter.setImage("output", settings.framebuffer->normal, oidn::Format::Float3, settings.width, settings.height);
    normalFilter.commit();

    // Prefilter the auxiliary images
    albedoFilter.execute();
    normalFilter.execute();

    // Filter the beauty image
    filter.execute();

    const char* errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << "Error: " << errorMessage << std::endl;

    for (unsigned int i = 0; i < settings.width * settings.height * 3; i++) {
        settings.beauty[i] = outColor[i];
    }

    finsihedDenoising->store(true);
    //shutdown
    free(outColor);
    free(image);
}

/*Frame ray_colorI(const ray& r, const hittable& world, int max_depth) {
    vec3 color(1.0, 1.0, 1.0);

    ray cur_ray = r;

    bool anyhit = false;

    for (int i = 0; i < max_depth; i++) {
        hit_record rec;
        if (world.hit(cur_ray, 0.0001, infinity, rec)) {
            anyhit = true;
            ray scattered;
            vec3 attenuation;
            if (rec.mat->scatter(cur_ray, rec, attenuation, scattered)) {
                color *= attenuation;
                cur_ray = scattered;
            }
            else {
                color = vec3(0.0, 0.0, 0.0);
                break;
            }
        }
        else {
            vec3 unit_direction = unit_vector(cur_ray.direction());
            float t = 0.5f * (unit_direction.y() + 1.0f);
            vec3 c = (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
            color *= c;
        }
    }

    if (!anyhit) color = vec3(0.0, 0.0, 0.0);

    Frame frame;
    frame.color = color;
}*/



void render_pixel(int x, int y, int image_width, int image_height, int samples, int max_depth, Camera c, hittable* world, FrameBuffer* frame){
    vec3 color(0, 0, 0);
    vec3 albedo(0, 0, 0);
    vec3 normal(0, 0, 0);
    for (int s = 0; s < samples; s++) {
        auto u = (x + random_double()) / (image_width-1);
        auto v = (y + random_double()) / (image_height-1);
        ray r = c.get_ray(u, v);


        Frame f = ray_colorR(r, world, max_depth);
        color += f.color;
        albedo += f.albedo;
        normal += f.normal;
    }

    setPixel(x, y, image_width, image_height, color / samples, frame->color, true);
    setPixel(x, y, image_width, image_height, albedo / samples, frame->albedo, true);
    setPixel(x, y, image_width, image_height, unit_vector(normal / samples), frame->normal, true);
}

void thread_manager(int id, int jump_value, int image_width, int image_height, int samples, int max_depth, Camera c, hittable* world, FrameBuffer* frame){
    int index = id;
    while(index < image_width * image_height){
        render_pixel(index % image_width , index / image_width, image_width, image_height, samples, max_depth, c, world, frame);
        index += jump_value;
    }
    std::cout << "Thread " << id << ": DONE" << std::endl;
}

void render(RenderSettings settings, std::atomic_bool* finished){
    std::clock_t start = std::clock();
    
    if (MULTI_THREADED) {
        const unsigned int numThreads = 16;
        std::thread t[numThreads];

        std::cout << "Beginning\nTotal threads: " << numThreads << std::endl;

        for (int i = 0; i < numThreads; i++) t[i] = std::thread(thread_manager, i, numThreads, settings.width, settings.height, settings.samples, settings.max_depth, settings.cam, settings.world, settings.framebuffer);
        for (int i = 0; i < numThreads; i++) t[i].join();
    }
    else {
        for (unsigned int y = 0; y < settings.height; y++) {
            std::cout << "Scanlines completed: " << y << '\r' << std::flush;
            for (unsigned int x = 0; x < settings.width; x++) {
                render_pixel(x, y, settings.width, settings.height, settings.samples, settings.max_depth, settings.cam, settings.world, settings.framebuffer);
            }
        }
    }

    finished->store(true);
    std::cout << "ALL DONE!" << std::endl;
    std::clock_t end = std::clock();
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
    << (end-start) / CLOCKS_PER_SEC << " s\n";
}
