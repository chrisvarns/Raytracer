#include <chrono>
#include <array>
#include <future>

#include "raytracer.h"
#include "glm/gtc/random.hpp"
#include "material.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "textures/checker_texture.h"
#include "textures/constant_texture.h"
#include "movingsphere.h"
#include "sphere.h"

hitable_list basic_scene() {
    hitable_list list;
    list.hitables.reserve(1000);
    list.hitables.push_back(new sphere(vec3(0,-1000,0), 1000, new lambertian(new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9))))));
    list.hitables.push_back(new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5)));
    list.hitables.push_back(new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1)))));
    list.hitables.push_back(new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0)));

    return list;
}

hitable_list random_scene() {
    hitable_list list;
    list.hitables.reserve(1000);
    list.hitables.push_back(new sphere(vec3(0,-1000,0), 1000, new lambertian(new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9))))));
    for(int a = -11; a < 11; a++) {
        for(int b = -11; b < 11; b++) {
            float choose_mat = fastrandF();
            vec3 center(a+0.9*fastrandF(),0.2,b+0.9*fastrandF());
            if(length(center-vec3(4,0.2,0)) > 0.9) {
                if(choose_mat < 0.8) { // diffuse
                    list.hitables.push_back(new movingsphere(center, center+vec3(0, 0.5*fastrandF(), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(fastrandF()*fastrandF(), fastrandF()*fastrandF(), fastrandF()*fastrandF())))));
                }
                else if (choose_mat < 0.95) { // metal
                    list.hitables.push_back(new sphere(center, 0.2, new metal(vec3(0.5*(1+fastrandF()), 0.5*(1+fastrandF()), 0.5*(1+fastrandF())), 0.5*fastrandF())));
                }
                else { // glass
                    list.hitables.push_back(new sphere(center, 0.2, new dielectric(1.5)));
                }
            }
        }
    }
    list.hitables.push_back(new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5)));
    list.hitables.push_back(new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1)))));
    list.hitables.push_back(new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0)));

    return list;
}

bvh_node convertListToBvh(const hitable_list& list, float time0, float time1) {
    return bvh_node(const_cast<const hitable**>(list.hitables.data()), int(list.hitables.size()), time0, time1);
}

vec3 blue(float(186) / float(255), float(205) / float(255), float(247) / float(255));
vec3 red(float(220) / float(255), float(205) / float(255), float(226) / float(255));

vec3 color(const ray& r, hitable& world, int depth) {
    hit_record rec;
    if(world.hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0);
        }
    }
    else {
        vec3 unit_direction = normalize(r.direction());
        float t = 0.5 * (unit_direction.x + 1);
        return (1-t)*blue + t*red;
    }
}

raytracer::raytracer() {
    blue = blue * blue;
    red = red * red;

    list = random_scene();
    world = convertListToBvh(list, cam.time0, cam.time1);
}

void raytracer::setSize(int width, int height) {
    width_ = width;
    height_ = height;

    colorAccumulator.clear();
    colorAccumulator.resize(width * height);

    num_iterations_ = 0;
    total_mrays_ = 0;

    cam = camera(lookfrom, lookat, vec3(0,1,0), 20, float(width_)/height_, aperture, dist_to_focus, 0.0, 0.4);
}

void raytracer::drawFrame(U8* outPtr) {
    ray::resetRayCount();
    num_iterations_++;

    auto start = std::chrono::steady_clock::now();

    const int numThreads = 8;
    std::array<std::future<void>, numThreads> futures;

    int perThreadHeight = height_ / numThreads;
    assert((height_ % numThreads) == 0);

    for(int threadIdx = 0; threadIdx < numThreads; threadIdx++) {

        futures[threadIdx] = std::async(std::launch::async, [=]{
            auto offset = width_ * perThreadHeight * threadIdx;
            auto colorAccumulatorItr = colorAccumulator.begin() + offset;
            auto writePtr = outPtr + offset * 4;

            auto jOffset = perThreadHeight * threadIdx;
            for(auto j = jOffset;
                j < jOffset + perThreadHeight; j++) {
                float v = float(j + fastrandF()) / height_;

                for(auto i = 0; i < width_; i++) {
                    float u = float(i + fastrandF()) / width_;

                    ray r = cam.get_ray(u, v);
                    vec3 col = sqrt(color(r, world, 0));

                    vec3& accumulatedCol = *colorAccumulatorItr;
                    accumulatedCol += col;
                    colorAccumulatorItr++;

                    col = accumulatedCol / vec3(num_iterations_);
                    *(writePtr++) = U8(255.99 * col.r);
                    *(writePtr++) = U8(255.99 * col.g);
                    *(writePtr++) = U8(255.99 * col.b);
                    *(writePtr++) = 255;
                }
            }

        });
    }

    for(auto& future : futures) future.wait();

    const float millionth = 1.0e-6f;
    const float billionth = 1.0e-9f;

    auto end = std::chrono::steady_clock::now();

    auto diff = end - start;
    float seconds = float(diff.count()) * billionth;
    float mrays = float(ray::rayCount()) / seconds * millionth;
    total_mrays_ += mrays;
    printf("MRays/s: %.4f\t\tAverage: %.4f\n", mrays, total_mrays_ / num_iterations_);
}
