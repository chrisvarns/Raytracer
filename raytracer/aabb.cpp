#include "aabb.h"

bool aabb::hit(const ray &r, float tmin, float tmax) const {
    for (int a = 0; a < 3; a++) {
        float invD = 1.0f / r.direction()[a];

        float t0 = (min[a] - r.origin()[a]) * invD;
        float t1 = (max[a] - r.origin()[a]) * invD;
        if(invD < 0.0f)
            std::swap(t0, t1);

        tmin = ffmax(t0, tmin);
        tmax = ffmin(t1, tmax);
        if(tmax <= tmin)
            return false;
    }
    return true;
}

aabb surrounding_box(aabb& box0, aabb& box1) {
    vec3 small = min(box0.min, box1.min);
    vec3 big = max(box0.max, box1.max);
    return aabb(small, big);
}
