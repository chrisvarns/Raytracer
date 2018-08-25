#pragma once
#include "material.h"

struct dielectric : public material {
    dielectric(float ri) : ref_idx(ri) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;
    vec3 emission(const hit_record& rec) const override { return vec3(0); }

    float ref_idx;
};
