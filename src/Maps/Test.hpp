#pragma once
#include "../World.hpp"

namespace Armed
{

namespace Maps
{

namespace Test
{

const char map_data[]=
"                                                                             \n"
"               bbbbbddd         iiiii         gggg                           \n"
"               aaahhaac         hhhhh         eeee                           \n"
"               aaahaac          jjhhj         eeee                           \n"
"               cccccc             jj          ffff                           \n"
"                                                                             \n"
"                           o                                                 \n"
"                                 A B C                                       \n"
"            n     4  5           A B C        p                             #\n"
"          ############         ###################                          #\n"
"      01 ##           # 89:;  #                     ##~~~~~~~~              #\n"
"      ###              #######   m     m              ~~~~~~~~########      #\n"
"     #      #     m #          ##########             ~~~~~~~~              #\n"
" ####       #########                    &&&&&&&&&&   ~~~~~~~~             %#\n"
"########################### #################################################\n";

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
