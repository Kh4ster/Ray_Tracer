#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"	// CUDA includes
#include "curand_kernel.h"

#include "Vector3.hpp"
#include "Device_Intersection.hpp"

class Device_Metal
{
public:
    __device__
    static bool scatter(Device_Intersection& intersection, Ray& scattered)
    {
        Vector3 reflected = reflect(intersection.get_ray()->get_direction(), intersection.get_normal());
        scattered = Ray(intersection.get_hit_location(), reflected);
        return (dot(scattered.get_direction(), intersection.get_normal()) > 0); //TODO optimize, get normal right away
    }

    __device__
    static Vector3 reflect(const Vector3& v, const Vector3& n)
    {
        return v - n * 2 * dot(v, n);
    }
};