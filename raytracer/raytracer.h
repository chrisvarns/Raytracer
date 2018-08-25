#pragma once
#include <array>
#include <future>
#include "glm/glm.hpp"
#include "types.h"
#include "hitablelist.h"
#include "bvh_node.h"
#include "camera.h"

using namespace glm;

struct raytracer {
    static const int numThreads = 8;

    raytracer();

    void setSize(int width, int height);
    void drawFrame(U8* output);

private:
    void doLoadBalancing(std::array<std::future<float>, numThreads>& futures);

    std::vector<vec3> colorAccumulator;
    std::array<int, numThreads> perThreadAllotment;

    camera cam;
    vec3 lookfrom = vec3(13,2,3);
    vec3 lookat = vec3(0,0.5f,0);
    float dist_to_focus = 10;
    float aperture = 0.1f;

    int width_ = -1;
    int height_ = -1;

    int num_iterations_ = 0;
    float total_mrays_ = 0;

    hitable_list list;
    bvh_node world;
};
