#pragma once

#define BLUE Color{ 0.0f, 0.0f, 1.0f }
#define WHITE Color{ 1.0f, 1.0f, 1.0f }
#define BLACK Color{ 0.0f, 0.0f, 0.0f }

#include <cmath>
#include <algorithm>

class Color
{
public:

	Color() : r_(0.0f), g_(0.0f), b_(0.0f) {}

	Color(float l) : r_(l), g_(l), b_(l) {}

	Color(float r, float g, float b) : r_(r), g_(g), b_(b) {}

	void clamp(float min, float max)
	{
		r_ = std::max(min, std::min(max, r_));
		g_ = std::max(min, std::min(max, g_));
		b_ = std::max(min, std::min(max, b_));
	}

	void apply_gamma_correction(float exposure, float gamma)
	{
		r_ = std::pow(r_ * exposure, gamma);
		g_ = std::pow(g_ * exposure, gamma);
		b_ = std::pow(b_ * exposure, gamma);
	}

	Color& operator+=(const Color& c)
	{
		r_ += c.get_r();
		g_ += c.get_g();
		b_ += c.get_b();
		return *this;
	}

	Color& operator*=(const Color& c)
	{
		r_ *= c.get_r();
		g_ *= c.get_g();
		b_ *= c.get_b();
		return *this;
	}

	Color& operator*=(float f)
	{
		r_ *= f;
		g_ *= f;
		b_ *= f;
		return *this;
	}
    
	float get_r() const { return r_; }
	float get_g() const { return g_; }
	float get_b() const { return b_; }

private:
	float r_, g_, b_;

};

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