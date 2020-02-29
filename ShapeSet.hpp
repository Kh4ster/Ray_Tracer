#pragma once

#include <vector>

#include "Shape.hpp"
#include "Intersection.hpp"

class ShapeSet
{
public:

	void add_shape(Shape* shape)
	{
		shapes_.push_back(shape);
	}

	bool intersect(Intersection& intersection)
	{
		bool intersected = false;
		for (auto shape : shapes_)
		{
			if (shape->intersect(intersection))
				intersected = true;
		}

		return intersected;
	}

private:
	std::vector<Shape*> shapes_;
};