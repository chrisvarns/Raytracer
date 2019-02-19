#pragma once
#include "hitable.h"

class xy_rect : public hitable
{
public:
	xy_rect() {}
	xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material* mat)
		: x0(_x0)
		, x1(_x1)
		, y0(_y0)
		, y1(_y1)
		, k(_k)
		, mat(mat)
	{
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;

	material* mat;
	float x0, x1, y0, y1, k;
};