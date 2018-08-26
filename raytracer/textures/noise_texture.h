#pragma once
#include "texture.h"
#include "perlin.h"

struct noise_texture : public texture {
    noise_texture(float scale) : scale_(scale) {}
    vec3 value(float u, float v, const vec3& p) const {
        return vec3(1) * noise_.noise(scale_ * p);
    }

    perlin noise_;
    float scale_;
};
