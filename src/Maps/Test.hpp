#pragma once
#include "../World.hpp"

namespace Armed
{

namespace Maps
{

namespace Test
{

const char map_data[]=
"                                                    \n"
"                           o                        \n"
"                                                    \n"
"            n                            m          \n"
"          ############         #####################\n"
"      0  ##           #                             \n"
"      ###              #######   m     m            \n"
"     #      #     m #           ##########          \n"
" ####       #########                               \n";

const MapDescription description
{
	map_data,
	{ 27 * g_fixed16_one, -1 * g_fixed16_one },
	{
		{ { {23, 4}, {26, 4} } },
		{ { {26, 1}, {23, 1} } },
		{ { {28, 2}, {28, 0} } },
		{ { {30, 0}, {30, 2} } },
	},
};

} // namespace Test

} // namespace Maps

} // namespace Armed
