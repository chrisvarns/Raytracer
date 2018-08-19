#include "camera.h"
#include "hitablelist.h"
#include "material.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "movingsphere.h"
#include "raytracer.h"
#include "sphere.h"
#include "glm/gtc/random.hpp"

hitable* random_scene() {
    int n = 100;
    hitable** list = new hitable*[n+1];
    list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5)));
    int i = 1;
    for(int a = -4; a < 4; a++) {
        for(int b = -4; b < 4; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            if(length(center-vec3(4,0.2,0)) > 0.9) {
                if(choose_mat < 0.8) { // diffuse
                    list[i++] = new movingsphere(center, center+vec3(0, 0.5*drand48(), 0), 0.0, 1.0, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.5*drand48()));
                }
                else { // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    
    return new hitable_list(list, i);
}

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
    const int ns = 20;
#endif
    
    hitable* world = random_scene();
    
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10;
    float aperture = 0.0;
    camera cam(lookfrom, lookat, vec3(0,1,0), 20.0, float(width)/height, aperture, dist_to_focus, 0.0, 1.0);
    
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
