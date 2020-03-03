#pragma once

#include "Ray.hpp"
#include "Vector3.hpp"
#include "device_atomic_functions.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes
#include "curand_kernel.h"

#include "math_util.hpp"
#include "Device_Intersection.hpp"
#include <ctime>

class Device_Lambertian
{
public:
    __device__
    static void scatter(const Device_Intersection& intersection, Ray& scattered)
    {
        Vector3 target = intersection.get_hit_location() + intersection.get_normal() + random_hit_mate();
        scattered = Ray(intersection.get_hit_location(), target - intersection.get_hit_location());
    }

    __device__
    static Vector3 random_hit_mate(void)
    {
        curandState_t state;
        curand_init(0, 0, 0, &state);

        Vector3 v;
        do
        {
            v = Vector3(curand_uniform(&state), curand_uniform(&state), curand_uniform(&state)) * 2.0 - Vector3(1.0f, 1.0f, 1.0f);
        } while (v.norm_no_sqrt() >= 1.0f);

        return v;
    }

};