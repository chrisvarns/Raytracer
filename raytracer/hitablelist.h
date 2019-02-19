#pragma once
#include <vector>
#include "hitable.h"
#include "shapes/sphere.h"
#include "shapes/movingsphere.h"

struct hitable_list : public hitable {
    hitable_list() {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& box) const override;
    
    std::vector<hitable*> hitables;
};
