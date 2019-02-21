#include "box.h"
#include "shapes/rects/xy_rect.h"
#include "shapes/rects/xz_rect.h"
#include "shapes/rects/yz_rect.h"
#include "flip_normals.h"

box::box(const vec3& p0, const vec3& p1, material* ptr)
{
	pmin = p0;
	pmax = p1;

	list.hitables.reserve(6);
	list.hitables.push_back(new xy_rect(p0.x, p1.x, p0.y, p1.y, p1.z, ptr));
	list.hitables.push_back(new flip_normals(new xy_rect(p0.x, p1.x, p0.y, p1.y, p0.z, ptr)));
	list.hitables.push_back(new xz_rect(p0.x, p1.x, p0.z, p1.z, p1.y, ptr));
	list.hitables.push_back(new flip_normals(new xz_rect(p0.x, p1.x, p0.z, p1.z, p0.y, ptr)));
	list.hitables.push_back(new yz_rect(p0.y, p1.y, p0.z, p1.z, p1.x, ptr));
	list.hitables.push_back(new flip_normals(new yz_rect(p0.y, p1.y, p0.z, p1.z, p0.x, ptr)));
}

bool box::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	return list.hit(r, t_min, t_max, rec);
}

bool box::bounding_box(float t0, float t1, aabb& box) const
{
	box = aabb(pmin, pmax);
	return true;
}