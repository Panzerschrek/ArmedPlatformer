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

const Model platform
{
	{
		{
			{ -g_fixed16_one * 3 / 8, { -g_fixed16_one * 3 / 4, +g_fixed16_one * 3 / 4 } },
			{ 0, { -g_fixed16_one, +g_fixed16_one } },
		},
		ColorRGB(128, 128, 128),
	},
	{
		{
			{ 0, { -g_fixed16_one, +g_fixed16_one } },
			{ +g_fixed16_one * 3 / 8, { -g_fixed16_one * 3 / 4, +g_fixed16_one * 3 / 4 } },
		},
		ColorRGB(140, 140, 140),
	},
};

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

const Model monster_gunner
{
	{
		{
			{ -g_fixed16_one , { g_fixed16_one / 2, g_fixed16_one / 2 } },
			{ -g_fixed16_one / 2, { 0, +g_fixed16_one / 2, } },
		},
		ColorRGB(255, 255, 0),
	},
	{
		{
			{ -g_fixed16_one * 3 / 4 , { -g_fixed16_one / 2, -g_fixed16_one / 2 } },
			{ -g_fixed16_one / 2, { -g_fixed16_one * 5 / 12, 0 } },
		},
		ColorRGB(255, 255, 0),
	},
	{
		{
			{ -g_fixed16_one / 2 , { g_fixed16_one / 2, g_fixed16_one / 2 } },
			{ -g_fixed16_one / 4, { g_fixed16_one / 2, g_fixed16_one * 5 / 4 } },
		},
		ColorRGB(220, 220, 0),
	},
	{
		{
			{ -g_fixed16_one / 4, { g_fixed16_one / 2, g_fixed16_one * 5 / 4 } },
			{ +g_fixed16_one / 4 , { g_fixed16_one / 2, g_fixed16_one / 2 } },
		},
		ColorRGB(220, 220, 0),
	},
	{
		{
			{ +g_fixed16_one / 4, { 0, g_fixed16_one / 2 } },
			{ +g_fixed16_one * 3 / 4 , { g_fixed16_one / 2, g_fixed16_one / 2 } },
		},
		ColorRGB(255, 255, 0),
	},
	{
		{
			{ +g_fixed16_one / 4, { -g_fixed16_one / 2, 0 } },
			{ +g_fixed16_one / 2 , { -g_fixed16_one * 3 / 4, -g_fixed16_one * 3 / 4 } },
		},
		ColorRGB(255, 255, 0),
	},
	{
		{
			{ -g_fixed16_one / 2, { -g_fixed16_one * 5 / 12, g_fixed16_one / 2 } },
			{ 0 , { -g_fixed16_one / 4, g_fixed16_one / 2 } },
		},
		ColorRGB(255, 255, 0),
	},
	{
		{
			{ 0, { -g_fixed16_one / 4 , g_fixed16_one / 2} },
			{ g_fixed16_one / 4 , { -g_fixed16_one / 2, g_fixed16_one / 2 } },
		},
		ColorRGB(255, 255, 0),
	},
};

#define MONSTER_SCALE 3 / 2
const Model monster_bomber
{
	{
		{
			{ -g_fixed16_one / 2 * MONSTER_SCALE, { -g_fixed16_one * MONSTER_SCALE, -g_fixed16_one / 2 * MONSTER_SCALE } },
			{ -g_fixed16_one / 4 * MONSTER_SCALE, { -g_fixed16_one * MONSTER_SCALE, -g_fixed16_one / 4 * MONSTER_SCALE } },
		},
		ColorRGB(32, 128, 255),
	},
	{
		{
			{ -g_fixed16_one / 2 * MONSTER_SCALE, { 0, 0 } },
			{ -g_fixed16_one / 4 * MONSTER_SCALE, { -g_fixed16_one / 4 * MONSTER_SCALE, +g_fixed16_one / 4 * MONSTER_SCALE } },
		},
		ColorRGB(32, 128, 255),
	},
	{
		{
			{ -g_fixed16_one / 2 * MONSTER_SCALE, { g_fixed16_one / 4 * MONSTER_SCALE, +g_fixed16_one / 2 * MONSTER_SCALE } },
			{ -g_fixed16_one / 4 * MONSTER_SCALE, { g_fixed16_one / 4 * MONSTER_SCALE, g_fixed16_one * MONSTER_SCALE } },
		},
		ColorRGB(32, 128, 255),
	},
	{
		{
			{ -g_fixed16_one / 4 * MONSTER_SCALE, { -g_fixed16_one * MONSTER_SCALE, +g_fixed16_one * MONSTER_SCALE } },
			{ 0, { -g_fixed16_one * MONSTER_SCALE, +g_fixed16_one * MONSTER_SCALE } },
		},
		ColorRGB(32, 128, 255),
	},
	{
		{
			{ 0, { -g_fixed16_one * MONSTER_SCALE, -g_fixed16_one / 4 * MONSTER_SCALE } },
			{ g_fixed16_one / 4 * MONSTER_SCALE, { -g_fixed16_one / 4 * MONSTER_SCALE , -g_fixed16_one / 4 * MONSTER_SCALE } },
		},
		ColorRGB(32, 128, 255),
	},
	{
		{
			{ 0, { g_fixed16_one * 3 / 4 * MONSTER_SCALE, g_fixed16_one * MONSTER_SCALE } },
			{ g_fixed16_one / 4 * MONSTER_SCALE, { g_fixed16_one * MONSTER_SCALE, g_fixed16_one * MONSTER_SCALE } },
		},
		ColorRGB(32, 128, 255),
	},
};
#undef MONSTER_SCALE

const Model small_health
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

const Model key0
{
	{
		{
			{ -g_fixed16_one / 3, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
		},
		ColorRGB(224, 224, 224),
	},
	{
		{
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
			{ +g_fixed16_one / 3, { 0, +g_fixed16_one / 3 } },
		},
		ColorRGB(32, 32, 32),
	},
};

const Model key1
{
	{
		{
			{ -g_fixed16_one / 3, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
		},
		ColorRGB(224, 224, 224),
	},
	{
		{
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
			{ +g_fixed16_one / 3, { -g_fixed16_one / 3, 0 } },
		},
		ColorRGB(32, 32, 32),
	},
};

const Model key2
{
	{
		{
			{ -g_fixed16_one / 3, { 0, +g_fixed16_one / 3 } },
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
		},
		ColorRGB(224, 224, 224),
	},
	{
		{
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
			{ +g_fixed16_one / 3, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
		},
		ColorRGB(32, 32, 32),
	},
};

const Model key3
{
	{
		{
			{ -g_fixed16_one / 3, { -g_fixed16_one / 3, 0 } },
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
		},
		ColorRGB(224, 224, 224),
	},
	{
		{
			{ 0, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
			{ +g_fixed16_one / 3, { -g_fixed16_one / 3, +g_fixed16_one / 3 } },
		},
		ColorRGB(32, 32, 32),
	},
};

} // namespace Models

} // namespace Armed
