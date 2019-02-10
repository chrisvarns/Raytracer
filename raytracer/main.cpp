#include "raytracer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

const int width = 640;
const int height = 480;

void reorder(U8* output) {

	const auto lineSize = width * 4;
	char tmp[lineSize];

	for (int firstIdx = 0; firstIdx < height / 2; firstIdx++) {
		auto lastIdx = height - firstIdx - 1;
		auto* first = output + lineSize * firstIdx;
		auto* last = output + lineSize * lastIdx;
		memcpy(tmp, first, lineSize);
		memcpy(first, last, lineSize);
		memcpy(last, tmp, lineSize);
	}
}

int main() {
	raytracer _raytracer;
	_raytracer.setSize(width, height);

	U8* output = (U8*)malloc(width * height * 4);

	while (true)
	{
		_raytracer.drawFrame(output);
		reorder(output);
		stbi_write_png("output.png", width, height, 4, output, 0);
	}
}