#pragma once
#include "Color.hpp"
#include "Fixed.hpp"
#include <vector>

namespace Armed
{

struct ModelTrapezoidSide
{
	fixed16_t y;
	fixed16_t x[2];
};

struct ModelTrapezoid
{
	ModelTrapezoidSide sides[2];
	color_t color;
};

using Model= std::vector<ModelTrapezoid>;

} // namespace Armed
