#pragma once
#include "glm/glm.hpp"

using namespace glm;

struct ray {
    ray() {}
    ray(const vec3& a, const vec3& b) { this->a = a; this->b = b; }
    
    vec3 origin() const { return a; }
    vec3 direction() const { return b; }
    vec3 point_at_parameter(float t) const { return a + b * t; }
    
    vec3 a, b;
};
