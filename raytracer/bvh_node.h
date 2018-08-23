#pragma once
#include "hitable.h"

struct bvh_node : public hitable {
    bvh_node() {}
    bvh_node(const hitable** l, int n, float time0, float time1);

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& box) const override;

    aabb box;
    const hitable* left;
    const hitable* right;
};
