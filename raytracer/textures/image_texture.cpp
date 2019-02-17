#include "image_texture.h"

vec3 image_texture::value(float u, float v, const vec3& p) const
{
	int i = (u) * nx;
	int j = (1 - v) * ny - 0.001;
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	if (i > nx - 1) i = nx - 1;
	if (j > nx - 1) j = nx - 1;
	float r = int(data[3 * i + 3 * nx*j + 0]) / 255.0;
	float g = int(data[3 * i + 3 * nx*j + 1]) / 255.0;
	float b = int(data[3 * i + 3 * nx*j + 2]) / 255.0;
	return vec3(r, g, b);
}