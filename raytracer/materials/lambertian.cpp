#include "lambertian.h"
#include "glm/gtc/random.hpp"

bool lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 target = rec.p + rec.normal + ballRand(1.0f);
    scattered = ray(rec.p, target-rec.p, r_in.time());
    attenuation = albedo;
    return true;
}
