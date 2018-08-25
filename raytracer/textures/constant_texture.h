#pragma once

#include "../texture.h"

struct constant_texture : public texture {
    constant_texture() {}
    constant_texture(const vec3& color) : color_(color) {}

    vec3 value(float u, float v, const vec3& p) const override
    {
        return color_;
    };

    vec3 color_;
};
