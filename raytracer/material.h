#pragma once
#include "glm/glm.hpp"
#include "hitable.h"
using namespace glm;

struct material {
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};
