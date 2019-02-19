#pragma once

#include "hitable.h"

struct movingsphere : public hitable {
    movingsphere() {};
    movingsphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material* mat) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat(mat) {}
    
    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& box) const override;
    
    vec3 center(float time) const;
    
    material* mat;
    vec3 center0, center1;
    float time0, time1;
    float radius;
};
