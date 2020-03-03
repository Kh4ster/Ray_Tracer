#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes
#include "curand_kernel.h"

#include <iostream>
#include <cmath>

#include "math_util.hpp"

class Vector3;

float dot(const Vector3& v1, const Vector3& v2);
Vector3 cross(const Vector3& v1, const Vector3& v2);
float sqr(float v);
std::ostream& operator<<(std::ostream& out, const Vector3& v);

class Vector3
{
public:

	using Point3 = Vector3; // Points and vectors are basically the same thing + simplifies calculation between them
	using Vector2 = Vector3; // To handle vectors on the scene
	using Point2 = Vector2; // To handle points on the scene

	__device__ __host__
	Vector3(float x, float y, float z = 0) : x_(x), y_(y), z_(z) {}
	__device__ __host__
	Vector3() : x_(0.0f), y_(0.0f), z_(0.0f) {}

	__device__ __host__
	Vector3 operator*(float l) const
	{
		return Vector3(x_ * l, y_ * l, z_ * l);
	}

	__device__ __host__
	Vector3& operator*=(float l)
	{
		x_ *= l;
		y_ *= l;
		z_ *= l;

		return *this;
	}

	__device__ __host__
	Vector3& operator-=(float l)
	{
		x_ -= l;
		y_ -= l;
		z_ -= l;

		return *this;
	}

	__device__ __host__
	Vector3& operator-=(const Vector3& v)
	{
		x_ -= v.x_;
		y_ -= v.y_;
		z_ -= v.z_;

		return *this;
	}

	__device__ __host__
	Vector3 operator+(const Vector3 &v) const
	{
		return Vector3(x_ + v.x_, y_ + v.y_, z_ + v.z_);
	}

	__device__ __host__
	Vector3 operator-(const Vector3& v) const
	{
		return Vector3(x_ - v.x_, y_ - v.y_, z_ - v.z_);
	}

	__device__ __host__
	Vector3 operator/(float l) const
	{
		return Vector3(x_ / l, y_ / l, z_ / l);
	}

	// Used when the norm is squared, we aboid to sqrt then sqr
	__device__ __host__
	float norm_no_sqrt() const
	{
		return sqr(x_) + sqr(y_) + sqr(z_);
	}

	__device__ __host__
	float norm() const
	{
		return std::sqrt(norm_no_sqrt());
	}

	__device__ __host__
	Vector3 normalize() const
	{
		return *this / norm();
	}

	__device__ __host__
	float get_x() const { return x_; }
	__device__ __host__
	float get_y() const { return y_; }
	__device__ __host__
	float get_z() const { return z_; }

	 
private:
	float x_;
	float y_;
	float z_;
};

__device__ __host__
float dot(const Vector3& v1, const Vector3& v2)
{
	return v1.get_x() * v2.get_x() + v1.get_y() * v2.get_y() + v1.get_z() * v2.get_z();
}

__device__ __host__
Vector3 cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.get_y() * v2.get_z() - v1.get_z() * v2.get_y(),
		v1.get_z() * v2.get_x() - v1.get_x() * v2.get_z(),
		v1.get_x() * v2.get_y() - v1.get_y() * v2.get_x());
}

__device__ __host__
float sqr(float v) { return v * v; }

__device__ __host__
std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << "(" << v.get_x() << "," << v.get_y() << "," << v.get_z() << ")" << std::endl;
}