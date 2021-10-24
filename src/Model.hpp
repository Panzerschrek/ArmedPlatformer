#pragma once
#include "Fixed.hpp"
#include <vector>

namespace Armed
{

using pixel_t= uint32_t;

struct ModelTrapezoidSide
{
	fixed16_t y;
	fixed16_t x[2];
};

struct ModelTrapezoid
{
	ModelTrapezoidSide sides[2];
	pixel_t color;
};

using Model= std::vector<ModelTrapezoid>;

} // namespace Armed
