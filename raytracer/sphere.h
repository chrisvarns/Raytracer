#pragma once

#include "hitable.h"

struct sphere : public hitable {
    sphere() {};
    sphere(vec3 cen, float r, material* mat) : mat(mat), center(cen), radius(r) {}
    
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    
    vec3 center;
    material* mat;
    float radius;
};
