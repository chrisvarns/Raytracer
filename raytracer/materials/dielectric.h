#pragma once
#include "material.h"

struct dielectric : public material {
    dielectric(float ri) : ref_idx(ri) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

    float ref_idx;
};
