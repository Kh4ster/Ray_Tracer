#pragma once

#include "Shape.hpp"
#include "Vector3.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

#include <cmath>

class Sphere : public Shape
{
public:
	Sphere(const Point3& center, float radius, const Color& color)
													: center_(center),
													radius_(radius),
													color_(color)
													{}

	virtual bool intersect(Intersection& intersection) override
	{
		// Transform ray so we can consider origin-centred sphere
		Ray local_ray = intersection.get_ray();
		local_ray.center_origin(center_);

		// Calculate quadratic coefficients
		float a = local_ray.get_direction().norm_no_sqrt(); //TODO opti get_direction copy
		float b = 2 * dot(local_ray.get_direction(), local_ray.get_origin()); //same
		float c = local_ray.get_origin().norm_no_sqrt() - sqr(radius_); //same

		// Check whether we intersect
		float discriminant = sqr(b) - 4 * a * c;

		if (discriminant < 0.0f)
			return false;

		// Find two points of intersection, t1 close and t2 far
		float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

		// First check if close intersection is valid
		if (t1 > RAY_T_MIN && t1 < intersection.get_coeff())
			intersection.set_coeff(t1);
		else if (t2 > RAY_T_MIN && t2 < intersection.get_coeff())
			intersection.set_coeff(t2);
		else
			return false;

		// Finish populating intersection
		intersection.set_shape(this);
		intersection.set_color(color_);

		return true;
	}

private:
	Point3 center_;
	float radius_;
	Color color_;
};