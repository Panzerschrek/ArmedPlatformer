#pragma once
#include "../World.hpp"

namespace Armed
{

namespace Maps
{

namespace Map00
{

const char map_data[]=
"                                                           \n"
"b                                                          \n"
"a                                                          \n"
"a                                                          \n"
"a 8                             bbbbb                   bbb\n"
"abbbbbb                         aaaaa                   aaa\n"
"aaaaaaab                        caaac                   caa\n"
"aaaaaaaab                        cac                     aa\n"
"aaaaaaaaab                        A                      aa\n"
"aaaaaaaaaab                       A              m     % aa\n"
"abbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
;

const MapDescription description
{
	map_data,
	{ 18 * g_fixed16_one, 6 * g_fixed16_one },
	{
		//{ { {23, 4}, {26, 4} } },
		//{ { {26, 1}, {23, 1} } },
		//{ { {30, 2}, {30, 0} } },
		//{ { {32, 0}, {32, 2} } },
		//{ { { 3, 1}, { 3, 15} } },
	},
};

} // namespace Test

} // namespace Map00

} // namespace Armed
