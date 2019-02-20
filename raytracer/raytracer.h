#pragma once
#include <array>
#include <future>

#include "glm/glm.hpp"
#include "types.h"
#include "hitablelist.h"
#include "bvh_node.h"
#include "camera.h"

using namespace glm;

class SDL_Renderer;
class SDL_Texture;

struct raytracer {
    static const int numThreads = 4;

    raytracer();

	void setSize(int width, int height);
	void setupScene();

	struct startParams
	{
		U8* output;
		SDL_Renderer* renderer;
		SDL_Texture* texture;
	};
	std::future<void> start(startParams params);

	void requestStop() { stopRequested = true; }

private:
	void mainthread();
	void workerthread();

	void set_camera_spheres();
	void set_camera_cornellbox();

	void accumulate(U8* output, std::vector<vec3>& filledMem);
	std::vector<vec3> colorAccumulator;
	int num_iterations_ = 0;

    camera cam;
	vec3 lookfrom, lookat;
	float dist_to_focus, aperture, vfov;

    int width_ = -1;
    int height_ = -1;

    hitable_list list;
    bvh_node bvh;
	hitable* world;

	startParams params_;

	std::vector<std::vector<vec3>*> availableMems;
	std::mutex availableMems_mutex;
	std::condition_variable availableMems_cv;

	std::vector<std::vector<vec3>*> filledMems;
	std::mutex filledMems_mutex;
	std::condition_variable filledMems_cv;
	
	std::atomic_bool stopRequested = false;
};
