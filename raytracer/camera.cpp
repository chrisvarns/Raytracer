#include "camera.h"

#include "glm/glm.hpp"

using namespace glm;

camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
    vec3 u, v, w;
    float theta = vfov * M_PI/180;
    float half_height = tan(theta/2);
    float half_width = aspect * half_height;
    origin = lookfrom;
    w = normalize(lookfrom - lookat);
    u = normalize(cross(vup, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width*u - half_height*v - w;
    horizontal = 2*half_width*u;
    vertical = 2*half_height*v;
}

ray camera::get_ray(float u, float v) {
    return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
}
