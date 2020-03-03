#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes
#include "curand_kernel.h"

#include <cmath>

#include "Camera.hpp"
#include "Vector3.hpp"
#include "math_util.hpp"

class Device_PerspectiveCamera
{
public:
	__device__  __host__
	Device_PerspectiveCamera(const Point3& origin, const Vector3& target, float fov, float aspectRatio)
	{
		origin_ = origin;
		forward_ = (target - origin).normalize();
		right_ = cross(forward_, Vector3(0.0f, 1.0f, 0.0f)).normalize();
		up_ = cross(right_, forward_);

		h_ = tan(fov);
		w_ = h_ * aspectRatio;
	}

	__device__
	Ray make_ray(const Point2& point) const
	{
		Vector3 direction = forward_ + right_ * point.get_x() * w_ + up_ * point.get_y() * h_;

		return Ray(origin_, direction.normalize());
	}

private:
	Point3 origin_;
	Vector3 forward_;
	Vector3 up_;
	Vector3 right_;

	float h_;
	float w_;
};