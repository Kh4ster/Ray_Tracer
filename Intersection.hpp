#pragma once

#include <limits>

#include "Ray.hpp"
#include "Color.hpp"
#include "Vector3.hpp"

class Shape;

class Intersection
{
public:

	Intersection(const Ray& ray) : coeff_ray_collision_(std::numeric_limits<float>::max()), ray_(ray), shape_(nullptr) {}

	bool intersected() const
	{
		return shape_ != nullptr;
	}

	void set_ray(const Ray& r) { ray_ = r; }
	void set_shape(Shape* s) { shape_ = s; }
	void set_color(const Color& c) { color_ = c; }
	void set_coeff(float c) { coeff_ray_collision_ = c; hit_location_ = ray_.point_at_parameter(c); }
	void set_normal(const Vector3& normal_vector) { normal_ = normal_vector; }

	Color get_color() const { return color_; }
	Ray get_ray() const { return ray_; }
	float get_coeff() const { return coeff_ray_collision_; }
	Vector3 get_hit_location() const { return hit_location_; }
	Vector3 get_normal() const { return normal_; }
	Shape* get_shape() { return shape_; }

private:
	float coeff_ray_collision_;
	Ray ray_;
	Point3 hit_location_;
	Vector3 normal_;
	Shape* shape_;
	Color color_;
};