#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes
#include "curand_kernel.h"

#include <limits>
#include <float.h>

#include "Ray.hpp"
#include "Device_Color.hpp"
#include "Vector3.hpp"

struct Device_Sphere;

struct Device_Intersection
{
	__device__ __host__
	Device_Intersection(Ray* ray) : coeff_ray_collision_(FLT_MAX), ray_(ray), shape_(nullptr) {}

	__device__ __host__
	void set_ray(Ray* r) { ray_ = r; }
	__device__ __host__
	void set_shape(Device_Sphere* s) { shape_ = s; }
	__device__ __host__
	void set_color(const Device_color& c) { color_ = c; }
	__device__ __host__
	void set_coeff(float c) { coeff_ray_collision_ = c; hit_location_ = ray_->point_at_parameter(c); }
	__device__ __host__
	void set_normal(const Vector3& normal_vector) { normal_ = normal_vector; }

	__device__ __host__
	Device_color get_color() const { return color_; }
	__device__ __host__
	Ray* get_ray() { return ray_; }
	__device__ __host__
	float get_coeff() const { return coeff_ray_collision_; }
	__device__ __host__
	Vector3 get_hit_location() const { return hit_location_; }
	__device__ __host__
	Vector3 get_normal() const { return normal_; }
	__device__ __host__
	Device_Sphere* get_shape() { return shape_; }

	float coeff_ray_collision_;
	Ray* ray_;
	Point3 hit_location_;
	Vector3 normal_;
	Device_Sphere* shape_;
	Device_color color_;
};