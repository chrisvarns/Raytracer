#pragma once
#include "texture.h"
class image_texture : public texture
{
public:
	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B)
		: data(pixels)
		, nx(A)
		, ny(B)
	{}

	virtual vec3 value(float u, float v, const vec3& p) const;
	unsigned char* data;
	int nx, ny;
};

