#pragma once
#include "../texture.h"

struct checker_texture : public texture {
    checker_texture() {}
    checker_texture(const texture* t0, const texture* t1)
    :   even_(t0)
    ,   odd_(t1)
    {}

    vec3 value(float u, float v, const vec3& p) const override;

    const texture* even_;
    const texture* odd_;
};
