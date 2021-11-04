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
"                          ###                       \n"
"            n                            m          \n"
"          ############         #####################\n"
"      0  ##           #                             \n"
"      ###              #######   m     m            \n"
"     #      #     m #           ##########          \n"
" ####       #########                               \n";

const MapDescription description
{
	map_data,
	{ 3 * g_fixed16_one, -1 * g_fixed16_one }
};

} // namespace Test

} // namespace Maps

} // namespace Armed
