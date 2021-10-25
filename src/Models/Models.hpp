#pragma once
#include "../Model.hpp"

namespace Armed
{

inline constexpr color_t ColorRGB(const uint8_t r, const uint8_t g, const uint8_t b)
{
	return color_t((b << 0u) | (g << 8u) | (r << 16u));
}

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
