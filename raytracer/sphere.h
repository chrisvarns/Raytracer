#pragma once

#include "hitable.h"
#include "material.h"

struct sphere : public hitable {
    sphere() {};
    sphere(vec3 cen, float r, material* mat) : mat(mat), center(cen), radius(r) {}
    
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

    material* mat;
    vec3 center;
    float radius;
};
