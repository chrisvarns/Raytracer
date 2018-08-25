#include "emissive.h"
#include "glm/gtc/random.hpp"
#include "texture.h"

bool emissive::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    return false;
}

vec3 emissive::emission(const hit_record& rec) const {
    return emission_->value(0, 0, rec.p);
}
