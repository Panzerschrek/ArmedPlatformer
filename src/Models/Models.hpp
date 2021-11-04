#pragma once
#include "../Model.hpp"

namespace Armed
{

namespace Models
{

#define PLAYER_SCALE 7 / 20
constexpr color_t c_player_color= ColorRGB(255, 0, 255);

const Model player
{
	{
		{
			{ -g_fixed16_one * 5 / 2 * PLAYER_SCALE, { 0, 0 } },
			{ -g_fixed16_one * 3 / 2 * PLAYER_SCALE, { -g_fixed16_one * 2 / 3 * PLAYER_SCALE, -g_fixed16_one / 2 * PLAYER_SCALE } },
		},
		c_player_color,
	},
	{
		{
			{ -g_fixed16_one * 3 / 2 * PLAYER_SCALE, { -g_fixed16_one * 2 / 3 * PLAYER_SCALE, g_fixed16_one * 2 * PLAYER_SCALE } },
			{ +g_fixed16_one / 2 * PLAYER_SCALE, { -g_fixed16_one * 2 * PLAYER_SCALE, g_fixed16_one * PLAYER_SCALE } },
		},
		c_player_color,
	},
	{
		{
			{ -g_fixed16_one / 2 * PLAYER_SCALE, { -g_fixed16_one * PLAYER_SCALE, 0 } },
			{ +g_fixed16_one * 5 / 2 * PLAYER_SCALE, { -g_fixed16_one * PLAYER_SCALE, -g_fixed16_one * PLAYER_SCALE } },
		},
		c_player_color,
	},
	{
		{
			{ -g_fixed16_one / 2 * PLAYER_SCALE, { 0, g_fixed16_one * PLAYER_SCALE } },
			{ +g_fixed16_one * 5 / 2 * PLAYER_SCALE, { 0, g_fixed16_one * PLAYER_SCALE } },
		},
		c_player_color,
	},
};
#undef PLAYER_SCALE

#define MONSTER_SCALE 7 / 8

const Model monster_biter
{
	{
		{
			{ -g_fixed16_one * MONSTER_SCALE, { g_fixed16_one / 2 * MONSTER_SCALE, +g_fixed16_one / 2 * MONSTER_SCALE } },
			{ 0, { 0, g_fixed16_one * MONSTER_SCALE } },
		},
		ColorRGB(64, 64, 255),
	},
	{
		{
			{ 0, { 0, 0 } },
			{ +g_fixed16_one * MONSTER_SCALE, { -g_fixed16_one / 2 * MONSTER_SCALE, +g_fixed16_one / 2 * MONSTER_SCALE } },
		},
		ColorRGB(32, 32, 128),
	},
};
#undef MONSTER_SCALE

const Model power_up
{
	{
		{
			{ -g_fixed16_one / 4, { 0, 0 } },
			{ 0, { -g_fixed16_one / 4, +g_fixed16_one / 4 } },
		},
		ColorRGB(200, 16, 16),
	},
	{
		{
			{ 0, { -g_fixed16_one / 4, +g_fixed16_one / 4 } },
			{ +g_fixed16_one / 4, { 0, 0 } },
		},
		ColorRGB(200, 200, 200),
	},
};

} // namespace Models

} // namespace Armed
