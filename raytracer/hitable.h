#pragma once

#include "ray.h"

struct material;

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    material* mat;
};

struct hitable {
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};