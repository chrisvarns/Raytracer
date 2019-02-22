#pragma once
#include "hitable.h"

class rotate_y : public hitable
{
public:
	rotate_y(hitable* p, float angle);
	
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;

	hitable* ptr;
	float sin_theta, cos_theta;
	bool hasbox;
	aabb bbox;
};

