#pragma once
#include "hitable.h"

class yz_rect : public hitable
{
public:
	yz_rect() {}
	yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material* mat)
		: y0(_y0)
		, y1(_y1)
		, z0(_z0)
		, z1(_z1)
		, k(_k)
		, mat(mat)
	{
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;

	material* mat;
	float y0, y1, z0, z1, k;
};