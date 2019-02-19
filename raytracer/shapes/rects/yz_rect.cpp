#include "shapes/rects/yz_rect.h"

bool yz_rect::hit(const ray& r, float t0, float t1, hit_record& rec) const
{
	float t = (k - r.origin().x) / r.direction().x;
	if (t < t0 || t > t1)
		return false;

	float y = r.origin().y + t * r.direction().y;
	float z = r.origin().z + t * r.direction().z;
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;

	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat = mat;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(1, 0, 0);
	return true;
}

bool yz_rect::bounding_box(float t0, float t1, aabb& box) const
{
	box = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
	return true;
}
