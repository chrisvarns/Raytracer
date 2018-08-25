#pragma once
#include "material.h"

struct texture;

struct lambertian : public material {
    lambertian(const texture* a) : albedo(a) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;
    vec3 emission(const hit_record& rec) const override { return vec3(0); }


    
    const texture* albedo;
};
