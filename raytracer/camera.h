#pragma once
#include "ray.h"

struct camera {
    camera(float vfov, float aspect);
    ray get_ray(float u, float v);
    
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
