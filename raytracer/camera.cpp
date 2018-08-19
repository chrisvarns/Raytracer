#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"

using namespace glm;

camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) {
    time0 = t0;
    time1 = t1;
    lens_radius = aperture / 2;
    float theta = vfov * M_PI/180;
    float half_height = tan(theta/2);
    float half_width = aspect * half_height;
    origin = lookfrom;
    w = normalize(lookfrom - lookat);
    u = normalize(cross(vup, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
    horizontal = 2*half_width*focus_dist*u;
    vertical = 2*half_height*focus_dist*v;
}

ray camera::get_ray(float s, float t) {
    vec2 rd = diskRand(lens_radius);
    vec3 offset = u * rd.x + v*rd.y;
    float time = time0 + drand48()*(time1-time0);
    return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset, time);
}
