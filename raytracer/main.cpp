#include "raytracer.h"
#include "stb/stb_image_write.h"
#include "SDL.h"

const int width = 640;
const int height = 480;

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = nullptr;
	window = SDL_CreateWindow("Raytracer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_SHOWN);
	assert(window);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, width, height);

	raytracer _raytracer;
	_raytracer.setSize(width, height);
	_raytracer.setupScene();

	U8* output = (U8*)malloc(width * height * 4);
	memset(output, 0, width*height * 4);

	raytracer::startParams params;
	params.output = output;
	params.renderer = renderer;
	params.texture = texture;
	auto main_thread = _raytracer.start(params);

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		if (SDL_WaitEvent(&event))
		{
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
	}
	_raytracer.requestStop();
	main_thread.wait();

	stbi_write_png("output.png", width, height, 4, output, 0);
	free(output);

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}