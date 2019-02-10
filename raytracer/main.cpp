#include "raytracer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "SDL.h"

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

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = nullptr;
	window = SDL_CreateWindow("Raytracer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		0);
	assert(window);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	raytracer _raytracer;
	_raytracer.setSize(width, height);

	U8* output = (U8*)malloc(width * height * 4);

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		}

		_raytracer.drawFrame(output);

		SDL_UpdateTexture(texture, NULL, output, width * 4);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	reorder(output);
	stbi_write_png("output.png", width, height, 4, output, 0);
	free(output);

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}