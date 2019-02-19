#pragma once
#include "hitable.h"

class xz_rect : public hitable
{
public:
	xz_rect() {}
	xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material* mat)
		: x0(_x0)
		, x1(_x1)
		, z0(_z0)
		, z1(_z1)
		, k(_k)
		, mat(mat)
	{
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;

	material* mat;
	float x0, x1, z0, z1, k;
};