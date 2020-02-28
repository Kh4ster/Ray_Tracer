#pragma once

#include <vector>
#include <sstream>
#include <fstream>

#include "Color.hpp"

class Image
{
public:
	Image(size_t w, size_t h) : width_(w), height_(h) { image_.reserve(w * h); }

	void set(size_t x, size_t y, Color c)
	{
		image_.insert(image_.begin() + y * width_ + x, std::move(c));
	}

	void create_image(std::string name, float exposure, float gamma) const
	{
		std::ofstream file(name);
		file << "P3\n" << width_ << " " << height_ << "\n255\n";
		for (size_t y = 0; y < height_; ++y)
		{
			for (size_t x = 0; x < width_; ++x)
			{
				Color curr = image_[y * width_ + x];
				curr.apply_gamma_correction(exposure, gamma);
				file << (unsigned)(curr.get_r() * 255.0f) << " " << (unsigned)(curr.get_g() * 255.0f) << " " << (unsigned)(curr.get_b() * 255.0f) << "\n";
			}
		}
	}

	size_t get_width() const { return width_; }
	size_t get_height() const { return height_;  }

private:
	std::vector<Color> image_;
	const size_t width_;
	const size_t height_;
};