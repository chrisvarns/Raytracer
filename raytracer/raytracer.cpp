#include "camera.h"
#include "hitablelist.h"
#include "raytracer.h"
#include "sphere.h"

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
    
#if DEBUG
    const int ns = 10;
#else
    const int ns = 100;
#endif
    
    hitable* list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable* world = new hitable_list(list, 2);
    camera cam;
    
    for(auto j = 0; j < height; j++) {
        for(auto i = 0; i < width; i++) {
            vec3 col(0);
            for(int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / width;
                float v = float(j + drand48()) / height;
                ray r = cam.get_ray(u, v);
                col += color(r, world);
            }
            col /= ns;
            *(outPtr++) = U8(255.99 * col.r);
            *(outPtr++) = U8(255.99 * col.g);
            *(outPtr++) = U8(255.99 * col.b);
            *(outPtr++) = 255;
        }
    }
}
