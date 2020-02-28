#pragma once

#include <vector>

#include "Shape.hpp"

class ShapeSet : public Shape
{
public:

	void add_shape(Shape* shape)
	{
		shapes_.push_back(shape);
	}

	virtual bool intersect(Intersection& intersection) override
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