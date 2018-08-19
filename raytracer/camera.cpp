#include "camera.h"

#include "glm/glm.hpp"

using namespace glm;

camera::camera(float vfov, float aspect) {
float theta = vfov * M_PI/180;
float half_height = tan(theta/2);
float half_width = aspect * half_height;
    lower_left_corner = vec3(-half_width, -half_height, -1);
    horizontal = vec3(2*half_width, 0, 0);
    vertical = vec3(0, 2*half_height, 0);
    origin = vec3(0);
}

ray camera::get_ray(float u, float v) {
    return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
}
