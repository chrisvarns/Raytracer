#include "camera.h"

#include "glm/glm.hpp"

using namespace glm;

camera::camera() {
    lower_left_corner = vec3(-2,-1,-1);
    horizontal = vec3(4, 0, 0);
    vertical = vec3(0, 2, 0);
    origin = vec3(0);
}

ray camera::get_ray(float u, float v) {
    return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
}
