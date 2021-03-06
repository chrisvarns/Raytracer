#pragma once
#include "texture.h"
#include "perlin.h"

struct noise_texture : public texture {
    noise_texture(float scale) : scale_(scale) {}
    vec3 value(float u, float v, const vec3& p) const {
        //return vec3(1) * 0.5f * (1.0f + noise_.turb(scale_ * p));
        //return vec3(1) * (noise_.turb(scale_ * p));
        return vec3(1) * 0.5f * (1 + sin(scale_*p.z + 10*noise_.turb(p)));
    }

    perlin noise_;
    float scale_;
};
