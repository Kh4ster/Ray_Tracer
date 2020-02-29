#include <vector>

#include "Image.hpp"
#include "PerspectiveCamera.hpp"
#include "Vector3.hpp"
#include "ShapeSet.hpp"
#include "Sphere.hpp"
#include "Engine.hpp"
#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Engine.hpp"

using Point3 = Vector3; // Points and vectors are basically the same thing + simplifies calculation between them
using Vector2 = Vector3; // To handle vectors on the scene
using Point2 = Vector2; // To handle points on the scene

void init_scene(ShapeSet& scene);

int main(void)
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

	return 0;
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
	scene.add_shape(&red_sphere_right);

	Sphere* center_mirror = new Sphere(Point3(0.0f, 0.0f, -1.0f),
		0.5f,
		new Metal(Vector3(0.95f, 0.95f, 0.95f)));
	scene.add_shape(center_mirror);
}