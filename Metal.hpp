#pragma once

#include "Vector3.hpp"
#include "Intersection.hpp"
#include "Material.hpp"

class Intersection;

class Metal : public Material
{
public:
    Metal(const Vector3& a) : albdeo_(a) {}

    virtual bool scatter(const Intersection& intersection, Vector3& attenuation, Ray& scattered) const override
    {
        Vector3 reflected = reflect(intersection.get_ray().get_direction(), intersection.get_normal());
        scattered = Ray(intersection.get_hit_location(), reflected);
        attenuation = albdeo_;
        return (dot(scattered.get_direction(), intersection.get_normal()) > 0); //TODO optimize, get normal right away
    }

private:
	Vector3 reflect(const Vector3& v, const Vector3& n) const
    {
		return v - n * 2 * dot(v, n);
	}
    Vector3 albdeo_;
};