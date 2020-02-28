#pragma once

#include "Shape.hpp"
#include "Vector3.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

class Plane : public Shape
{
public:

	Plane(const Point3& position, const Vector3& normal, const Color& color)
																			: position_(position),
																			normal_(normal),
																			color_(color) {}

	virtual bool intersect(Intersection& intersection) override
	{
		float dDotN = dot(intersection.get_ray().get_direction(), normal_); //TODO optimize copy get

		if (dDotN == 0.0f) // Parallel or embedded in the plane
			return false;

		// Find point of intersection
		float t = dot(position_ - intersection.get_ray().get_origin(), normal_)
			/ dDotN;

		if (t <= RAY_T_MIN || t >= intersection.get_coeff())
			return false;

		intersection.set_coeff(t);
		intersection.set_shape(this);
		intersection.set_color(color_);

		return true;
	}

private:
	Point3 position_;
	Vector3 normal_;
	Color color_;
};