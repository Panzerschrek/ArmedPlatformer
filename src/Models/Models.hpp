#pragma once
#include "../Model.hpp"

namespace Armed
{

namespace Models
{

const Model player
{
	{
		{
			{ -g_fixed16_one / 2, { -g_fixed16_one / 2, +g_fixed16_one / 4 } },
			{ +g_fixed16_one / 2, { -g_fixed16_one / 2, +g_fixed16_one / 2 } },
		},
		ColorRGB(255, 0, 255),
	},
};

const Model monster_biter
{
	{
		{
			{ -g_fixed16_one / 2, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
			{ +g_fixed16_one / 2, { -g_fixed16_one / 2, +g_fixed16_one / 2 } },
		},
		ColorRGB(64, 64, 255),
	},
	{
		{
			{ -g_fixed16_one * 3 / 4, { -g_fixed16_one / 4, +g_fixed16_one / 4 } },
			{ -g_fixed16_one / 2, { -g_fixed16_one / 3, +g_fixed16_one / 5 } },
		},
		ColorRGB(32, 32, 128),
	},
};

} // namespace Models

} // namespace Armed
