#pragma once

#include "glm/glm.hpp"

using namespace glm;

struct texture {
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};
