#pragma once
#include <vector>
#include "hitable.h"
#include "sphere.h"

struct hitable_list : public hitable {
    hitable_list() {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    
    std::vector<sphere> spheres;
};
