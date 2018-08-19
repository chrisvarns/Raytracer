#include "raytracer.h"
#include "ray.h"

float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if(discriminant < 0)
        return -1;
    return (-b - sqrt(discriminant)) / (2*a);
}

vec3 color(const ray& r) {
    float t = hit_sphere(vec3(0,0,-1), 0.5, r);
    if(t > 0.0)
    {
        vec3 N = normalize(r.point_at_parameter(t) - vec3(0,0,-1));
        return vec3(0.5)*(N+vec3(1));
    }
    vec3 unit_direction = normalize(r.direction());
    t = 0.5 * (unit_direction.y + 1);
    return (1-t)*vec3(1) + t*vec3(0.5, 0.7, 1.0);
}

void redraw(U8* outPtr, int width, int height) {
    vec3 lower_left_corner(-2, -1, -1);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0);
    for(auto j = 0; j < height; j++) {
        for(auto i = 0; i < width; i++) {
            float u = float(i) / width;
            float v = float(j) / height;
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);
            *(outPtr++) = U8(255.99 * col.r);
            *(outPtr++) = U8(255.99 * col.g);
            *(outPtr++) = U8(255.99 * col.b);
            *(outPtr++) = 255;
        }
    }
}
