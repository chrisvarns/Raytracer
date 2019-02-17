#include "lambertian.h"
#include "glm/gtc/random.hpp"
#include "texture.h"

bool lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 target = rec.normal + ballRand(1.0f);
    scattered = ray(rec.p, target, r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}
