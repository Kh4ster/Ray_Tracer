#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes
#include "curand_kernel.h"


#include "Vector3.hpp"
#include "Device_Color.hpp"
#include "Ray.hpp"
#include "math_util.hpp"
#include "Device_Material.hpp"
#include "Device_Intersection.hpp"

#include <cmath>
#include <iostream>

class Device_Sphere
{
public:
	__device__ __host__
	Device_Sphere(const Point3& center, float radius, Device_Material material)
		: center_(center),
		radius_(radius),
		material_(material)
	{}

	__device__ __host__
	float get_radius(void) const
	{
		return radius_;
	}

	__device__ __host__
	bool intersect(Device_Intersection& intersection)
	{
		// Transform ray so we can consider origin-centred sphere
		Ray* local_ray = intersection.get_ray();
		local_ray->center_origin(center_);

		// Calculate quadratic coefficients
		float a = local_ray->get_direction().norm_no_sqrt(); //TODO opti get_direction copy
		float b = 2 * dot(local_ray->get_direction(), local_ray->get_origin()); //same
		float c = local_ray->get_origin().norm_no_sqrt() - sqr(radius_); //same

		// Check whether we intersect
		float discriminant = sqr(b) - 4 * a * c;

		if (discriminant < 0.0f)
			return false;

		// Find two points of intersection, t1 close and t2 far
		float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

		// First check if close intersection is valid
		if (t1 > RAY_T_MIN&& t1 < intersection.get_coeff() && t1 < RAY_T_MAX)
			intersection.set_coeff(t1);
		else if (t2 > RAY_T_MIN&& t2 < intersection.get_coeff() && t2 < RAY_T_MAX)
			intersection.set_coeff(t2);
		else
			return false;

		// Finish populating intersection
		intersection.set_shape(this);
		intersection.set_normal((intersection.get_hit_location() - center_) / radius_);

		return true;
	}
	
	__device__
	Device_Material get_material() const { return material_; }

private:
	Device_Material material_;
	Point3 center_;
	float radius_;
};