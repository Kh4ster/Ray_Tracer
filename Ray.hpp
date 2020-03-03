#pragma once

#include "Vector3.hpp"
#include "Color.hpp"

using Point3 = Vector3; // Points and vectors are basically the same thing + simplifies calculation between them
using Vector2 = Vector3; // To handle vectors on the scene
using Point2 = Vector2; // To handle points on the scene

constexpr auto RAY_T_MIN = 0.001f;
constexpr auto RAY_T_MAX = 1.0e30f;

class Ray
{
public:
	__device__ __host__
	Ray(const Point3& p, const Vector3& d) : origin_(p), direction_(d) {}
	__device__ __host__
	Ray() = default;

	__device__ __host__
	void center_origin(Point3 center)
	{
		origin_ -= center;
	}

	__device__ __host__
	Vector3 get_direction() { return direction_; }
	__device__ __host__
	Point3 get_origin() { return origin_; }

	__device__ __host__
	Point3 point_at_parameter(float t) const { return origin_ + direction_ * t; }

private:
	Point3 origin_;
	Vector3 direction_; // Normalized vector
};