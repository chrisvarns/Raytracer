#pragma once
#include "material.h"

struct metal : public material {
    metal(const vec3& a, float fuzz) : albedo(a), fuzz(clamp(fuzz, 0.0f, 1.0f)) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;
    vec3 emission(const hit_record& rec) const override { return vec3(0); }
    
    vec3 albedo;
    float fuzz;
};
