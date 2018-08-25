#pragma once
#include "material.h"
#include "texture.h"

struct texture;

struct emissive : public material {
    emissive(const texture* a) : emission_(a) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override;
    vec3 emission(const hit_record& rec) const override;
    
    const texture* emission_;
};
