#pragma once

#include "Intersection.hpp"

class Shape
{
public:
	virtual bool intersect(Intersection& intersection) = 0;
};