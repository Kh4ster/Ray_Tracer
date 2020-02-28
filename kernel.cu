#include <vector>

#include "Image.hpp"
#include "PerspectiveCamera.hpp"
#include "Vector3.hpp"
#include "ShapeSet.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Engine.hpp"

using Point3 = Vector3; // Points and vectors are basically the same thing + simplifies calculation between them
using Vector2 = Vector3; // To handle vectors on the scene
using Point2 = Vector2; // To handle points on the scene

int main(void)
{
	constexpr size_t width = 200;
	constexpr size_t height = 100;
	constexpr float pi = 3.14f;

	Image image(width, height);

	PerspectiveCamera camera(Point3(0.0f, 0.0f, -1.0f),
									Vector3(0.0f, 0.0f, -1.0f),
									25.0f * pi / 180.0f,
									static_cast<float>(width) / static_cast<float>(height));

	ShapeSet scene;
	Engine engine;

	Sphere sphere1(Point3(0.0f, 0.0f, -1.0f), 0.5f,
						Color(0.5f, 1.0f, 0.5f));
	scene.add_shape(&sphere1);

	Sphere sphere(Point3(0.0f, -100.5f, -1.0f), 100.0f,
		Color(0.5f, 0.5f, 1.0f));
	scene.add_shape(&sphere);

	engine.ray_trace(image, camera, scene);

	image.create_image("out_image.ppm", 1.0f, 2.2f);

	return 0;
}