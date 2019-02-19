#pragma once
#include "hitable.h"
class flip_normals : public hitable
{
public:
	flip_normals(hitable* p) : ptr(p) {}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;

	hitable* ptr;
};

