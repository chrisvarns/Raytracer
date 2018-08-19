#include "camera.h"
#include "hitablelist.h"
#include "material.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "raytracer.h"
#include "sphere.h"
#include "glm/gtc/random.hpp"

vec3 color(const ray& r, hitable* world, int depth) {
    hit_record rec;
    if(world->hit(r, 0.001, MAXFLOAT, rec)) {
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
        float t = 0.5 * (unit_direction.y + 1);
        return (1-t)*vec3(1) + t*vec3(0.5, 0.7, 1.0);
    }
}

void redraw(U8* outPtr, int width, int height) {
    
#if DEBUG
    const int ns = 4;
#else
    const int ns = 100;
#endif
    
    hitable* list[4];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
    hitable* world = new hitable_list(list, 4);
    camera cam;
    
    for(auto j = 0; j < height; j++) {
        for(auto i = 0; i < width; i++) {
            vec3 col(0);
            for(int s = 0; s < ns; s++) {
                float u = float(i + drand48()) / width;
                float v = float(j + drand48()) / height;
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= ns;
            col = sqrt(col);
            *(outPtr++) = U8(255.99 * col.r);
            *(outPtr++) = U8(255.99 * col.g);
            *(outPtr++) = U8(255.99 * col.b);
            *(outPtr++) = 255;
        }
    }
}
