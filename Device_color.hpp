#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes
#include "curand_kernel.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

#include "Vector3.hpp"

// Full public needed for cuda malloc/memcpy
struct Device_color
{

	__device__
	void set(unsigned x, unsigned y, float rval, float gval, float bval)
	{
		r[x + y * width] = rval;
		g[x + y * width] = gval;
		b[x + y * width] = bval;
	}

	__device__
	void add(unsigned x, unsigned y, const Device_color& c)
	{
		r[x + y * width] += c.r[x + y * width];
		g[x + y * width] += c.g[x + y * width];
		b[x + y * width] += c.b[x + y * width];
	}

	__device__
	void mul_color(unsigned x, unsigned y, const Device_color& c)
	{
		r[x + y * width] *= c.r[x + y * width];
		g[x + y * width] *= c.g[x + y * width];
		b[x + y * width] *= c.b[x + y * width];
	}

	__device__
	void mul(unsigned x, unsigned y, float f)
	{
		r[x + y * width] *= f;
		g[x + y * width] *= f;
		b[x + y * width] *= f;
	}

	float *r;
	float *g;
	float *b;
	unsigned width;
};