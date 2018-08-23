#pragma once

#include "hitable.h"
#include "material.h"

struct sphere : public hitable {
    sphere() {};
    sphere(vec3 cen, float r, material* mat) : mat(mat), center(cen), radius(r) {}
    
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& box) const override;

    material* mat;
    vec3 center;
    float radius;
};
