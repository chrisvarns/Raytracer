#include <chrono>

#include "glm/gtc/random.hpp"
#include "SDL.h"
#include "SDL_log.h"
#include "stb/stb_image.h"

#include "raytracer.h"
#include "material.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/diffuse_light.h"
#include "textures/checker_texture.h"
#include "textures/constant_texture.h"
#include "textures/noise_texture.h"
#include "textures/image_texture.h"
#include "shapes/movingsphere.h"
#include "shapes/sphere.h"
#include "shapes/rects/xy_rect.h"
#include "shapes/rects/xz_rect.h"
#include "shapes/rects/yz_rect.h"
#include "flip_normals.h"

hitable_list cornell_box()
{
	hitable_list list;
	list.hitables.reserve(6);

	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(25, 25, 25)));

	list.hitables.push_back(new flip_normals(new yz_rect(0, 555, 0, 555, 555, green)));
	list.hitables.push_back(new yz_rect(0, 555, 0, 555, 0, red));
	list.hitables.push_back(new xz_rect(213, 343, 227, 332, 554, light));
	list.hitables.push_back(new flip_normals(new xz_rect(0, 555, 0, 555, 555, white)));
	list.hitables.push_back(new xz_rect(0, 555, 0, 555, 0, white));
	list.hitables.push_back(new flip_normals(new xy_rect(0, 555, 0, 555, 555, white)));

	return list;
}

hitable_list simple_light_scene()
{
	texture* pertext = new noise_texture(4);
	hitable_list list;
	list.hitables.reserve(4);
	list.hitables.push_back(new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext)));
	list.hitables.push_back(new sphere(vec3(0, 2, 0), 2, new lambertian(pertext)));
	list.hitables.push_back(new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4,4,4)))));
	list.hitables.push_back(new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4,4,4)))));
	return list;
}

hitable_list globe_scene()
{	
	hitable_list list;
	list.hitables.reserve(1);
	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	list.hitables.push_back(new sphere(vec3(0, 0, 0), 2, new lambertian(new image_texture(tex_data, nx, ny))));
	return list;
}

hitable_list two_perlin_spheres() {
    material* lambert = new lambertian(new noise_texture(4));
    hitable_list list;
    list.hitables.reserve(2);
    list.hitables.push_back(new sphere(vec3(0,-1000,0), 1000, lambert));
    list.hitables.push_back(new sphere(vec3(0, 2, 0), 2, lambert));
    return list;
}

hitable_list basic_scene() {
    hitable_list list;
    list.hitables.reserve(1000);
    list.hitables.push_back(new sphere(vec3(0,-1000,0), 1000, new lambertian(new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9))))));
    list.hitables.push_back(new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5)));
    list.hitables.push_back(new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1)))));
    list.hitables.push_back(new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0)));

    return list;
}

hitable_list random_scene() {
    hitable_list list;
    list.hitables.reserve(1000);
    list.hitables.push_back(new sphere(vec3(0,-1000,0), 1000, new lambertian(new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9))))));
    for(int a = -11; a < 11; a++) {
        for(int b = -11; b < 11; b++) {
            float choose_mat = fastrandF();
            vec3 center(a+0.9*fastrandF(),0.2,b+0.9*fastrandF());
            if(length(center-vec3(4,0.2,0)) > 0.9) {
                if(choose_mat < 0.8) { // diffuse
                    list.hitables.push_back(new movingsphere(center, center+vec3(0, 0.5*fastrandF(), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(fastrandF()*fastrandF(), fastrandF()*fastrandF(), fastrandF()*fastrandF())))));
                }
                else if (choose_mat < 0.95) { // metal
                    list.hitables.push_back(new sphere(center, 0.2, new metal(vec3(0.5*(1+fastrandF()), 0.5*(1+fastrandF()), 0.5*(1+fastrandF())), 0.5*fastrandF())));
                }
                else { // glass
                    list.hitables.push_back(new sphere(center, 0.2, new dielectric(1.5)));
                }
            }
        }
    }
    list.hitables.push_back(new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5)));
    list.hitables.push_back(new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new noise_texture(10))));
	list.hitables.push_back(new sphere(vec3(-8, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0)));

	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	list.hitables.push_back(new sphere(vec3(4, 1, 0), 1, new lambertian(new image_texture(tex_data, nx, ny))));

    return list;
}

bvh_node convertListToBvh(const hitable_list& list, float time0, float time1) {
    return bvh_node(const_cast<const hitable**>(list.hitables.data()), int(list.hitables.size()), time0, time1);
}

vec3 blue(float(186) / float(255), float(205) / float(255), float(247) / float(255));
vec3 red(float(220) / float(255), float(205) / float(255), float(226) / float(255));

vec3 color(const ray& r, hitable& world, int depth) {
    hit_record rec;
    if(world.hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
		vec3 emitted = rec.mat->emitted(rec.u, rec.v, rec.p);
        if(depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered)) {
            return emitted + attenuation*color(scattered, world, depth+1);
        }
        else {
            return emitted;
        }
    }
    else {
		return vec3();
		/*vec3 unit_direction = normalize(r.direction());
		float t = 0.5 * (unit_direction.x + 1);
		return (1-t)*blue + t*red;*/
    }
}

raytracer::raytracer() {
    blue = blue * blue;
    red = red * red;
}

void raytracer::setupScene()
{
	//list = two_perlin_spheres();
	//list = random_scene();
	//list = globe_scene();
	//list = simple_light_scene();
	list = cornell_box();
	bvh = convertListToBvh(list, cam.time0, cam.time1);
	world = &bvh;
}

std::future<void> raytracer::start(startParams params)
{
	params_ = params;

	return std::async(std::launch::async, [this]() -> void
	{
		this->mainthread();
	});
}

void raytracer::set_camera_spheres()
{
	lookfrom = vec3(13, 2, 3);
	lookat = vec3(0, 0, 0);
	dist_to_focus = 10;
	aperture = 0;
	vfov = 45;
	vec3 world_up = vec3(0, 1, 0);

	cam = camera(lookfrom, lookat, world_up, vfov, float(width_) / height_, aperture, dist_to_focus, 0.0, 1.0);
}

void raytracer::set_camera_cornellbox()
{
	lookfrom = vec3(278, 278, -800);
	lookat = vec3(278, 278, 0);
	dist_to_focus = 10;
	aperture = 0;
	vfov = 40;
	vec3 world_up = vec3(0, 1, 0);

	cam = camera(lookfrom, lookat, world_up, vfov, float(width_) / height_, aperture, dist_to_focus, 0.0, 1.0);
}

void raytracer::setSize(int width, int height) {
    width_ = width;
    height_ = height;

    colorAccumulator.clear();
    colorAccumulator.resize(width * height);

    num_iterations_ = 0;

	//set_camera_spheres();
	set_camera_cornellbox();
}

void raytracer::mainthread()
{
	// Create the mems
	std::vector<std::vector<vec3>> workingMems;
	workingMems.resize(numThreads + 2);
	for (auto& workingMem : workingMems)
	{
		workingMem.resize(width_ * height_);
		availableMems.push_back(&workingMem);
	}

	// Kick off the workers
	std::vector<std::future<void>> workers;
	for (int i = 0; i < numThreads; ++i)
	{
		workers.push_back(std::async(std::launch::async, [this]() -> void
		{
			this->workerthread();
		}));
	}

	while (!stopRequested)
	{
		// Wait for work to be completed by workers
		std::vector<vec3>* filledMem = nullptr;
		{
			std::unique_lock<std::mutex> lock(filledMems_mutex);
			filledMems_cv.wait(lock, [this] { return filledMems.size() > 0; });
			filledMem = filledMems.back();
			filledMems.pop_back();
			lock.unlock();
		}

		accumulate(params_.output, *filledMem);

		{
			std::lock_guard<std::mutex> lock(availableMems_mutex);
			availableMems.push_back(filledMem);
		}
		availableMems_cv.notify_one();

		SDL_UpdateTexture(params_.texture, NULL, params_.output, width_ * 4);
		SDL_RenderCopy(params_.renderer, params_.texture, NULL, NULL);
		SDL_RenderPresent(params_.renderer);
	}

	for (auto& worker : workers)
	{
		worker.wait();
	}

	for (auto& filledMem : filledMems)
	{
		accumulate(params_.output, *filledMem);
	}
}

void raytracer::workerthread()
{
	while (!stopRequested)
	{
		std::vector<vec3>* workingMem = nullptr;
		{
			std::unique_lock<std::mutex> lock(availableMems_mutex);
			if (availableMems.size() == 0)
			{
				availableMems_cv.wait(lock, [this] { return availableMems.size() > 0; });
			}
			workingMem = availableMems.back();
			availableMems.pop_back();
			lock.unlock();
		}

		auto workingMemItr = workingMem->begin();

		for (auto j = 0; j < height_; j++)
		{
			float v = 1.0f - (float(j) + fastrandF()) / height_;

			for (auto i = 0; i < width_; i++)
			{
				float u = (float(i) + fastrandF()) / width_;

				ray r = cam.get_ray(u, v);
				*workingMemItr = sqrt(color(r, *world, 0));
				workingMemItr++;
			}
		}

		{
			std::lock_guard<std::mutex> lock(filledMems_mutex);
			filledMems.push_back(workingMem);
		}
		filledMems_cv.notify_one();
	}
}

void raytracer::accumulate(U8* output, std::vector<vec3>& filledMem)
{
	num_iterations_++;
	SDL_Log("Iteration %d", num_iterations_);
	auto writePtr = output;

	assert(filledMem.size() == colorAccumulator.size());

	for (int i = 0; i < filledMem.size(); ++i)
	{
		auto& accumulator_elem = colorAccumulator[i];
		auto& in_elem = filledMem[i];

		accumulator_elem += in_elem;
		vec3 col = accumulator_elem / vec3(num_iterations_);
		*(writePtr++) = U8(255.99 * col.r);
		*(writePtr++) = U8(255.99 * col.g);
		*(writePtr++) = U8(255.99 * col.b);
		*(writePtr++) = 255;
	}
}