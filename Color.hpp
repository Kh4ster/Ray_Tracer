#pragma once

#define BLUE Color{ 0.0f, 0.0f, 1.0f }
#define WHITE Color{ 1.0f, 1.0f, 1.0f }
#define BLACK Color{ 0.0f, 0.0f, 0.0f }

#include <cmath>
#include <algorithm>
#include <iostream>

#include "Vector3.hpp"

class Color
{
public:

	Color()
	{
		rgb[0] = 0.0f;
		rgb[1] = 0.0f;
		rgb[2] = 0.0f;
	}

	Color(float l)
	{
		rgb[0] = l;
		rgb[1] = l;
		rgb[2] = l;
	}

	Color(float r, float g, float b)
	{
		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;
	}

	void clamp(float min, float max)
	{
		rgb[0] = std::max(min, std::min(max, rgb[0]));
		rgb[1] = std::max(min, std::min(max, rgb[1]));
		rgb[2] = std::max(min, std::min(max, rgb[2]));
	}

	void apply_gamma_correction(float exposure, float gamma)
	{
		rgb[0] = std::pow(rgb[0] * exposure, gamma);
		rgb[1] = std::pow(rgb[1] * exposure, gamma);
		rgb[2] = std::pow(rgb[2] * exposure, gamma);
	}

	Color& operator+=(const Color& c)
	{
		rgb[0] += c.get_r();
		rgb[1] += c.get_g();
		rgb[2] += c.get_b();
		return *this;
	}

	Color& operator*=(const Color& c)
	{
		rgb[0] *= c.get_r();
		rgb[1] *= c.get_g();
		rgb[2] *= c.get_b();
		return *this;
	}

	Color& operator*=(float f)
	{
		rgb[0] *= f;
		rgb[1] *= f;
		rgb[2] *= f;
		return *this;
	}
    
	float get_r() const { return rgb[0]; }
	float get_g() const { return rgb[1]; }
	float get_b() const { return rgb[2]; }

private:
	float rgb[3];

};

inline std::ostream& operator<<(std::ostream& out, const Color& c)
{
	return out << "(" << c.get_r() << "," << c.get_g() << "," << c.get_b() << ")" << std::endl;
}

inline Color operator+(const Color& c1, const Color& c2)
{
	return Color(c1.get_r() + c2.get_r(),
		c1.get_g() + c2.get_g(),
		c1.get_b() + c2.get_b());
}

inline Color operator*(const Color& c1, const Color& c2)
{
	return Color(c1.get_r() * c2.get_r(),
		c1.get_g() * c2.get_g(),
		c1.get_b() * c2.get_b());
}

inline Color operator*(const Color& c, float f)
{
	return Color(c.get_r() * f,
		c.get_g() * f,
		c.get_b() * f);
}

inline Color operator*(const Color& c, const Vector3& v)
{
	return Color(c.get_r() * v.get_x(),
		c.get_g() * v.get_y(),
		c.get_b() * v.get_z());
}

inline Color operator/(const Color& c, float f)
{
	return Color(c.get_r() / f,
		c.get_g() / f,
		c.get_b() / f);
}

inline Color operator*(float f, const Color& c)
{
	return Color(f * c.get_r(),
		f * c.get_g(),
		f * c.get_b());
}