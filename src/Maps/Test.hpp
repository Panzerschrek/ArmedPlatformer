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
"          ############         ###################  \n"
"      0  ##           #                             \n"
"      ###              #######   m     m            \n"
"     #      #     m #           ##########          \n"
" ####       #########   ~~~~~             &&&&&&&&&&\n"
"####################################################\n";

const MapDescription description
{
	map_data,
	{ 1 * g_fixed16_one, -1 * g_fixed16_one },
	{
		//{ { {23, 4}, {26, 4} } },
		//{ { {26, 1}, {23, 1} } },
		//{ { {30, 2}, {30, 0} } },
		//{ { {32, 0}, {32, 2} } },
		//{ { { 3, 1}, { 3, 15} } },
	},
};

} // namespace Test

} // namespace Maps

} // namespace Armed
