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
"aaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbabbbbbbbbbbbbbbbbbbbbbbaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
;

const MapDescription description
{
	map_data,
	{ 18 * g_fixed16_one, 6 * g_fixed16_one },
	{
	},
};

} // namespace Test

} // namespace Map00

} // namespace Armed
