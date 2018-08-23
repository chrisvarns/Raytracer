#pragma once
#include "glm/glm.hpp"
#include "ray.h"

using namespace glm;

inline float ffmax(float a, float b) { return a > b ? a : b; }
inline float ffmin(float a, float b) { return a < b ? a : b; }

struct aabb {
    aabb() {}
    aabb(const vec3& a, const vec3& b) : min(a), max(b) {}

    bool hit(const ray& r, float tmin, float tmax) const;

    vec3 min;
    vec3 max;
};

aabb surrounding_box(aabb& box0, aabb& box1);
