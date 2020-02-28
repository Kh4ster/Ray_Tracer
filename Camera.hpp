#pragma once

#include "Ray.hpp"
#include "Vector3.hpp"

using Point3 = Vector3; // Points and vectors are basically the same thing + simplifies calculation between them
using Vector2 = Vector3; // To handle vectors on the scene
using Point2 = Vector2; // To handle points on the scene

class Camera
{
public:
	virtual Ray make_ray(const Vector2& point) const = 0;
};