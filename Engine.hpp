#pragma once

#include "Image.hpp"
#include "Camera.hpp"
#include "Shape.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "math_util.hpp"

using Point3 = Vector3; // Points and vectors are basically the same thing + simplifies calculation between them
using Vector2 = Vector3; // To handle vectors on the scene
using Point2 = Vector2; // To handle points on the scene

class Engine
{
public:
	void ray_trace(Image& image, const Camera& camera, ShapeSet& scene)
	{
		for (size_t y = 0; y < image.get_height(); y++)
		{
			for (size_t x = 0; x < image.get_width(); x++)
			{
				Color c;
				for (size_t i = 0; i < 100; ++i)
				{
					// Convert pixel x y, to screen point
					Point2 screen_coord((2.0f * static_cast<float>(x) + get_random_float()) / static_cast<float>(image.get_width()) - 1.0f,
						(-2.0f * static_cast<float>(y) + get_random_float()) / static_cast<float>(image.get_height()) + 1.0f);

					Ray ray = camera.make_ray(screen_coord);
					c += get_color(ray, scene, 0);
				}
				image.set(x, y, c / 100.0f);
			}
		}
	}
	

private:
	Color get_color(const Ray& ray, ShapeSet& scene, int depth)
	{
		Intersection intersection(ray);
		if (scene.intersect(intersection))
		{
			Ray scattered;
			Vector3 attenuation;
			if (depth < 50 && intersection.get_shape()->get_material()->scatter(intersection, attenuation, scattered)) //TODO better design
				return get_color(scattered, scene, depth + 1) * attenuation;
			else
				return BLACK;
		}
		else
			return WHITE;
	}

};