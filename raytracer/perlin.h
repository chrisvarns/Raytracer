#pragma once

#include "glm/glm.hpp"

using namespace glm;

struct perlin {
    perlin();
    
    float noise(const vec3& p) const;

    static vec3* ranvec;
    static int* perm_x;
    static int* perm_y;
    static int* perm_z;
};
