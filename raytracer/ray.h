#pragma once
#include <atomic>
#include "types.h"
#include "glm/glm.hpp"

using namespace glm;

struct ray {
    ray() {}
    ray(const vec3& a, const vec3& b, float time = 0.0) { this->a = a; this->b = b; this->time_ = time; rayCount_++; }
    
    vec3 origin() const { return a; }
    vec3 direction() const { return b; }
    float time() const { return time_; }
    vec3 point_at_parameter(float t) const { return a + b * t; }

    static void resetRayCount() { rayCount_ = 0; }
    static U32 rayCount() { return rayCount_; }

private:
    vec3 a, b;
    float time_;

    static std::atomic<U32> rayCount_;
};
