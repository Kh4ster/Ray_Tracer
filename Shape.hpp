#pragma once

#include "Intersection.hpp"
#include "Material.hpp"

class Shape
{
public:
	virtual bool intersect(Intersection& intersection) = 0;
	Material* get_material(void) { return material_; }

protected:
	Material* material_;
};