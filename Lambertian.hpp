#pragma once

#include "Vector3.hpp"
#include "math_util.hpp"
#include "Material.hpp"

class Lambertian : public Material
{
public:
    Lambertian(const Vector3& a) : albedo(a) {}
    
    virtual bool scatter(const Intersection& intersection, Vector3& attenuation, Ray& scattered) const override
    {    
        Vector3 target = intersection.get_hit_location() + intersection.get_normal() + random_hit_mate();
        scattered = Ray(intersection.get_hit_location(), target - intersection.get_hit_location());
        attenuation = albedo;
        return true;
    }

private:

    Vector3 random_hit_mate() const
    {
        Vector3 v;
        do
        {
            v = Vector3(get_random_float(), get_random_float(), get_random_float()) * 2.0 - Vector3(1.0f, 1.0f, 1.0f);
        } while (v.norm_no_sqrt() >= 1.0f);

        return v;
    }

    Vector3 albedo;
};