#pragma once

#include <cstdlib>

float get_random_float()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}