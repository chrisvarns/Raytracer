#include "Raytracer.hpp"
#include "glm/glm.hpp"
using namespace glm;

void redraw(U8* outPtr, int width, int height) {
        vec3 lower_left_corner(-2, -1, -1);
        vec3 horizontal(4, 0, 0);
        vec3 vertical(0, 2, 0);
        vec3 origin(0);
        for(auto j = 0; j < width; j++) {
            for(auto i = 0; i < height; i++) {
                float u = float(i) / _viewportSize.x;
                float v = float(j) / _viewportSize.y;
                ray r(origin, lower_left_corner + u*horizontal + v*vertical);
                vec3 col = color(r);
                *(outPtr++) = uint8(255.99 * col.r);
                *(outPtr++) = uint8(255.99 * col.g);
                *(outPtr++) = uint8(255.99 * col.b);
                *(outPtr++) = 255;
            }
        }
}
