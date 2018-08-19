#include "metal.h"
#include "glm/gtc/random.hpp"

bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
    vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + ballRand(fuzz));
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}
