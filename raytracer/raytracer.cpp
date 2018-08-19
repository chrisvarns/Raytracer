#include "raytracer.h"
#include "sphere.h"
#include "hitablelist.h"

vec3 color(const ray& r, hitable* world) {
    hit_record rec;
    if(world->hit(r, 0.0, MAXFLOAT, rec)) {
        return vec3(0.5)*(rec.normal+vec3(1));
    }
    else {
        vec3 unit_direction = normalize(r.direction());
        float t = 0.5 * (unit_direction.y + 1);
        return (1-t)*vec3(1) + t*vec3(0.5, 0.7, 1.0);
    }
}

void redraw(U8* outPtr, int width, int height) {
    vec3 lower_left_corner(-2, -1, -1);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0);
    
    hitable* list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable* world = new hitable_list(list, 2);
    
    for(auto j = 0; j < height; j++) {
        for(auto i = 0; i < width; i++) {
            float u = float(i) / width;
            float v = float(j) / height;
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r, world);
            *(outPtr++) = U8(255.99 * col.r);
            *(outPtr++) = U8(255.99 * col.g);
            *(outPtr++) = U8(255.99 * col.b);
            *(outPtr++) = 255;
        }
    }
}
