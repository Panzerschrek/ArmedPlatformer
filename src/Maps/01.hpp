#pragma once
#include "../World.hpp"

namespace Armed
{

namespace Maps
{

namespace Map01
{

const char map_data[]=
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaaccccccccccccccaaaaccccccccccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaa              caac            caaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaa               cc      o       cccccccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaa    bbbb                                aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaa    aaaabb                              aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaa    aaaaaabb                bbbbbbbb    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaa    aaaaaaaa                aaaaaaaa    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaa    aaaaaaaa                aaaaaaaa    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaacccccccccc    aaaaaaaa                aaaaaaaa    aaaaaaaccccccccccccccccccccaaa\n"
"aaa              aaaaaaaa                aaaaaaaa    caaaaac                    cca\n"
"aaa              aaaaaaaa                aaaaaaaa     ccccc       o               a\n"
"aaa              aaaaaaaa                aaaaaaaa                                 a\n"
"aaa    bbbbbb    aaaaaaaa                aaaaaaaa                                 a\n"
"aaa    aaaaaa    aaaaaaaa                aaaaaaaa     0  4                      % a\n"
"aaa    aaaaaa    aaaaaaaa                aaaaaaaa    bbbbbb              bbbbbbbbba\n"
"aaa    aaaaaa    aaaaaaaa                aaaaaaaa    aaaaaa              aaaaaaaaaa\n"
"aaa    aaaaaa    aaaaaaaa                aaaaaaaa    aaaaaa              aaaaaaaaaa\n"
"acc    aaaaaa    aaaaaaaa                aaaaaaaa    aaaaaa              aaaaaaaaaa\n"
"a      aaaaaa    aaaaaaaa                aaaaaaaa    aaaaaa              aaaaaaaaaa\n"
"a      aaaaaa    aaaaaaaa                aaaaaaaa    aaaaaa              aaaaaaaaaa\n"
"a      aaaaaa    aaaaaaaa                aaaaaaaa    aaaaaa              aaaaaaaaaa\n"
"abb    aaaaaa    aaaaaaaa                aaaaaaaa    aaaaaa              aaaaaaaaaa\n"
"aaa&&&&aaaaaa&&&&aaaaaaaa&&&&&&&&&&&&&&&&aaaaaaaa&&&&aaaaaa&&&&&&&&&&&&&&aaaaaaaaaa\n"
"aaa&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&aaaaaaaaaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa&&&&&&&&&&aaaaaaaaaaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
;

const MapDescription description
{
	map_data,
	{ 1 * g_fixed16_one, 21 * g_fixed16_one },
	{
		{ { { 5, 23 }, { 5, 12 } } },
		{ { { 15, 13 }, { 15, 3 } } },
		{ { { 26, 7 }, { 40, 7 } } },
		{ { { 51, 7 }, { 51, 16 } } },
		{ { { 60, 16 }, { 72, 16 } } },
	},
};

} // namespace Test

} // namespace Map01

} // namespace Armed
