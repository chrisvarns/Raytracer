#pragma once
#include "material.h"

struct texture;

struct lambertian : public material {
    lambertian(const texture* a) : albedo(a) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
    
    const texture* albedo;
};
