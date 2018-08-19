#pragma once
#include "material.h"

struct metal : public material {
    metal(const vec3& a) : albedo(a) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
    
    vec3 albedo;
};
