#include "sphere.h"
#include "glm/gtc/constants.hpp"

void get_sphere_uv(const vec3& p, float& u, float& v)
{
	const float pi = glm::pi<float>();
	float phi = atan(p.z, p.x);
	float theta = asin(p.y);
	u = 1 - (phi + pi) / (2 * pi);
	v = (theta + pi / 2) / pi;
}

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if(discriminant > 0) {
        float sqrt_discriminant = sqrt(discriminant);
        float temp = (-b - sqrt_discriminant) / a;
        if(temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat = mat;
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            return true;
        }
        temp = (-b + sqrt_discriminant) / a;
        if(temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat = mat;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const{
    vec3 r(radius);
    box = aabb(center - r, center + r);
    return true;
}