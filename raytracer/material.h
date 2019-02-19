#pragma once
#include "glm/glm.hpp"
#include "hitable.h"
using namespace glm;

struct material {
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
	virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(); }
};
