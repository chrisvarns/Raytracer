#pragma once
#include "hitable.h"

class translate : public hitable
{
public:
	translate(hitable* p, const vec3& displacement)
		: ptr(p)
		, offset(displacement)
	{
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;

	hitable* ptr;
	vec3 offset;
};

