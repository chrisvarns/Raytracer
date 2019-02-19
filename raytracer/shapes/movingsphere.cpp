#include "movingsphere.h"

bool movingsphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if(discriminant > 0) {
        float temp = (-b -sqrt(b*b-a*c)) / a;
        if(temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat = mat;
            return true;
        }
        temp = (-b + sqrt(b*b - a*c)) / a;
        if(temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat = mat;
            return true;
        }
    }
    return false;
}

vec3 movingsphere::center(float time) const {
    return center0 + ((time - time0) / (time1-time0)) * (center1-center0);
}

bool movingsphere::bounding_box(float t0, float t1, aabb& box) const {
    auto center_t0 = center(t0);
    auto center_t1 = center(t1);
    auto r = vec3(radius);
    auto box0 = aabb(center_t0 - r, center_t0 + r);
    auto box1 = aabb(center_t1 - r, center_t1 + r);
    box = surrounding_box(box0, box1);
    return true;
}
