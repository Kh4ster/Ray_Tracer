#ifndef __CUDA_SAFE_CALL
#define __CUDA_SAFE_CALL(call) do { cudaError_t cuda_error = call; if(cuda_error != cudaSuccess) { std::cerr << "CUDA Error: " << cudaGetErrorString(cuda_error) << ", " << __FILE__ << ", line " << __LINE__ << std::endl;} } while(0)
#endif

#include "device_atomic_functions.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes

#include "Image.hpp"
#include "PerspectiveCamera.hpp"
#include "Vector3.hpp"
#include "ShapeSet.hpp"
#include "Sphere.hpp"
#include "Engine.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Engine.hpp"
#include "Device_color.hpp"
#include "Device_Sphere.hpp"
#include "Device_Intersection.hpp"
#include "Device_Lambertian.hpp"
#include "Device_Metal.hpp"
#include "Device_PerspectiveCamera.hpp"

#include <iostream>
#include <vector>

using Point3 = Vector3; // Points and vectors are basically the same thing + simplifies calculation between them
using Vector2 = Vector3; // To handle vectors on the scene
using Point2 = Vector2; // To handle points on the scene

void init_scene(ShapeSet& scene);
void host_ray_tracer(void);
void device_ray_tracer(void);
__global__
void device_ray_trace(Device_color * image, const unsigned width, const unsigned height, Device_PerspectiveCamera * camera, Device_Sphere * scene, unsigned nb_sphere);

int main(void)
{
	std::cout << "Enter h to execute on host (CPU), d to execute on device (GPU)" << std::endl;
	
	char choice;
	std::cin >> choice;
	
	if (choice == 'h')
		host_ray_tracer();
	else if (choice == 'd')
		device_ray_tracer();

	return 0;
}

static void device_ray_tracer(void)
{
	constexpr size_t width = 1920;
	constexpr size_t height = 1080;
	constexpr float pi = 3.14f;
	constexpr float exposure = 1.0f;
	constexpr float gamma = 2.2f;
	constexpr unsigned nb_spheres = 4;
	constexpr unsigned warp_size = 32;

	Image image(width, height);

	Device_PerspectiveCamera h_camera(Point3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), 25.0f * pi / 180.0f, static_cast<float>(width) / static_cast<float>(height));
	Device_Sphere h_spheres[nb_spheres]
	{
		Device_Sphere(Point3(-1.0f, 0.0f, -1.0f), 0.5f, Device_Material::DEVICE_LAMB),
		Device_Sphere(Point3(0.0f, -100.5f, -1.0f), 100.0f, Device_Material::DEVICE_LAMB),
		Device_Sphere(Point3(1.0f, 0.0f, -1.0f), 0.5f, Device_Material::DEVICE_LAMB),
		Device_Sphere(Point3(0.0f, 0.0f, -1.0f), 0.5f, Device_Material::DEVICE_METAL)
	};
	Device_color h_color;
	float h_r[width * height];
	float h_g[width * height];
	float h_b[width * height];
	
	Device_PerspectiveCamera *d_camera;
	Device_Sphere *d_spheres;
	Device_color* d_color;
	float* d_r;
	float* d_g;
	float* d_b;

	dim3 block_size(warp_size, 1, 1);
	dim3 grid_size(width / warp_size + 1, height / warp_size + 1, 1);

	__CUDA_SAFE_CALL(cudaMalloc((void**)d_camera, sizeof(Device_PerspectiveCamera)));
	__CUDA_SAFE_CALL(cudaMalloc((void**)d_spheres, sizeof(Device_Sphere) * nb_spheres));
	__CUDA_SAFE_CALL(cudaMalloc((void**)d_color, sizeof(Device_color)));
	__CUDA_SAFE_CALL(cudaMalloc((void**)d_r, sizeof(float) * width * height));
	__CUDA_SAFE_CALL(cudaMalloc((void**)d_g, sizeof(float) * width * height));
	__CUDA_SAFE_CALL(cudaMalloc((void**)d_b, sizeof(float) * width * height));

	__CUDA_SAFE_CALL(cudaMemcpyAsync(d_camera, &h_camera, sizeof(Device_PerspectiveCamera), cudaMemcpyHostToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(d_spheres, h_spheres, sizeof(Device_Sphere) * nb_spheres, cudaMemcpyHostToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(&(d_color->r), d_r, sizeof(float*), cudaMemcpyDeviceToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(&(d_color->g), d_g, sizeof(float*), cudaMemcpyDeviceToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(&(d_color->b), d_b, sizeof(float*), cudaMemcpyDeviceToDevice));

	device_ray_trace<< <grid_size, block_size >> > (d_color, width, height, d_camera, d_spheres, nb_spheres);
	cudaDeviceSynchronize();

	__CUDA_SAFE_CALL(cudaMemcpyAsync(d_r, &(d_color->r), sizeof(float*), cudaMemcpyDeviceToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(d_g, &(d_color->g), sizeof(float*), cudaMemcpyDeviceToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(d_b, &(d_color->b), sizeof(float*), cudaMemcpyDeviceToDevice));
	
	__CUDA_SAFE_CALL(cudaMemcpyAsync(h_r, d_b, sizeof(float) * width * height, cudaMemcpyDeviceToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(h_g, d_r, sizeof(float) * width * height, cudaMemcpyDeviceToDevice));
	__CUDA_SAFE_CALL(cudaMemcpyAsync(h_b, d_g, sizeof(float) * width * height, cudaMemcpyDeviceToDevice));


	image.create_image("out_image.ppm", exposure, gamma);
}

__device__
void device_set_color(unsigned x, unsigned y, unsigned width, Device_color *color, Ray* ray, Device_Sphere* spheres, unsigned nb_sphere, int depth)
{
	bool intersect = false;
	Device_Intersection intersection(ray);
	for (unsigned i = 0; i < nb_sphere; ++i)
	{
		if (spheres[i].intersect(intersection))
		{
			if (spheres[i].material_ == Device_Material::DEVICE_LAMB)
			{
				Ray scattered;
				Device_Lambertian::scatter(intersection, scattered);
				if (depth < 2)
					device_set_color(x, y, width, color, &scattered, spheres, nb_sphere, depth + 1);
				else
					color->set(x, y, 0.0f, 0.0f, 0.0f);
			}
			else
			{
				Ray scattered;
				if (Device_Metal::scatter(intersection, scattered) && depth < 2)
					device_set_color(x, y, width, color, &scattered, spheres, nb_sphere, depth + 1);
				else
					color->set(x, y, 0.0f, 0.0f, 0.0f);
			}
			intersect = true;
		}
	}
	if (!intersect)
		color->set(x, y, 1.0f, 1.0f, 1.0f);
}

__global__
void device_ray_trace(Device_color* image, const unsigned width, const unsigned height, Device_PerspectiveCamera* camera, Device_Sphere* scene, unsigned nb_sphere)
{
	unsigned x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned y = blockIdx.y * blockDim.y + threadIdx.y;
	unsigned threadId = y * width + x;

	Point2 screen_coord((2.0f * static_cast<float>(x)) / static_cast<float>(width) - 1.0f,
		(-2.0f * static_cast<float>(y)) / static_cast<float>(height) + 1.0f);

	Ray ray = camera->make_ray(screen_coord);
	device_set_color(x, y, width, image, &ray, scene, nb_sphere, 0);
}

static void host_ray_tracer(void)
{
	constexpr size_t width = 1920;
	constexpr size_t height = 1080;
	constexpr float pi = 3.14f;
	constexpr float exposure = 1.0f;
	constexpr float gamma = 2.2f;

	Image image(width, height);

	PerspectiveCamera camera(Point3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), 25.0f * pi / 180.0f, static_cast<float>(width) / static_cast<float>(height));

	Engine engine;

	ShapeSet scene;
	init_scene(scene);

	engine.ray_trace(image, camera, scene);

	image.create_image("out_image.ppm", exposure, gamma);
}

static void init_scene(ShapeSet& scene)
{
	Sphere* red_sphere_left = new Sphere(Point3(-1.0f, 0.0f, -1.0f),
		0.5f,
		new Lambertian(Vector3(0.8f, 0.3f, 0.3f)));
	scene.add_shape(red_sphere_left);

	Sphere* bottom_big_sphere = new Sphere(Point3(0.0f, -100.5f, -1.0f),
		100.0f,
		new Lambertian(Vector3(0.8f, 0.3f, 0.3f)));
	scene.add_shape(bottom_big_sphere);

	Sphere* red_sphere_right = new Sphere(Point3(1.0f, 0.0f, -1.0f),
		0.5f,
		new Lambertian(Vector3(0.8f, 0.3f, 0.3f)));
	scene.add_shape(red_sphere_right);

	Sphere* center_mirror = new Sphere(Point3(0.0f, 0.0f, -1.0f),
		0.5f,
		new Metal(Vector3(0.95f, 0.95f, 0.95f)));
	scene.add_shape(center_mirror);
}