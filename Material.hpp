#pragma once

#include "Ray.hpp"
#include "Vector3.hpp"

class Intersection;

class Material 
{

public:
    virtual bool scatter(const Intersection& intersection, Vector3& attenuation, Ray& scattered) const = 0;

};