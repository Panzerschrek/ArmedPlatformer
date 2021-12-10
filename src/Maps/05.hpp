#pragma once
#include "../World.hpp"

namespace Armed
{

namespace Maps
{

namespace Map05
{

const char map_data[]=
"                                                                                                                                                           \n"
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb                                                                    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                                                    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                                                    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                                                    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbb  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeehhhhhhhhhhhhhhh  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh  hhhhhhhhhhhhhhheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n"
"eeffffffffffffffffffffffffffffffffffffffhhjjjjjjjjjjjjjj  jjjjhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhjjjj  jjjjjjjjjjjjjhhhffffffffffffffffffffffffffffffffffffffee\n"
"ee                                      jhj                   hhhhhhhjjjjjjjjjjjjjjjjjhhhhhhh                   jhj                                      ee\n"
"ee                   o                   j                    hhhhhhh                 hhhhhhh                    j                                       ee\n"
"ee                                                            D C B A                 A B C D                                                            ee\n"
"ee 9  0   n                     m                             D C B A 5 5 5 5 5 5 5 5 A B C D                       m          n             m       0 8 ee\n"
"eegggggggggggggggggggggggggggggggggggggggiiiiiiii  iiiiiiiiiiihihihihiiiiiiiiiiiiiiiihihihihiiiiiiiiiiii  iiiiiiiigggggggggggggggggggggggggggggggggggggggee\n"
"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeejhhhhhhh  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh  hhhhhhhheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhjjjjjjjj  jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj  jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh                                                                         hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh               o                                       o                 hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh                                                                         hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh                                                                         hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh                                                                         hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh   n 4           m             m      p       m            m       4 n   hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhiiiiiiii  iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii  iiiiiiiihhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj  jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj  jjjjjjjjjjjjjjjjjjjjjjhjjjjjjjjjjhhhjjjjjjjjjhhhh\n"
"hh                                                                                                                              j          jhj         jhhh\n"
"hh          o                o                                                                                                              j           jhh\n"
"hh                                                                                                                      i                                hh\n"
"hh                                                                                                                      hi                               hh\n"
"hh :                                   0       ii  ii       m        4   m           m                ii  ii       4    hhi ;   n    0     n             hh\n"
"hhiii                                iiiiiiiiiihhihhhiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii~~iiiiiiiiiiihhiihhiiiiiiiiiiiihhhiiiiiiiiiiiiiiiiiiiiiiiiiiii~~hh\n"
"hhhhh                                hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hh\n"
"hhhhh                                hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hhhhhhhhjjjjjjjjjjjjjhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hh\n"
"hhhhh                                hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hhhhhhhj~~~~~~~~~~~~~jhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hh\n"
"hhhhh                                hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hhhhhhj~~~~~~~~~~~~~~~jjjhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhj~~hh\n"
"hhhhh                                hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~hhhhhj~~~~~iiiiiiii~~~~~~jjjjjjjhhhhhhhhhhhjjjjhhhhhhhhhhhj~~~hh\n"
"hhhhh&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhj~~hhhhj~~~~~ihhhhhhhhi~~~~~~~~~~~~jjjjjjjjhjj~~~~jjjjjjjjjjj~~~~hh\n"
"hhhhh&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhj~~~jjjj~~~~~ihhhhhhhhhhii~~~~~~~~~~~~~~~~~~j~~~~~~~~~~~~~~~~~~~~~hh\n"
"hhhhh&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~~~~~~~~~~~ihhhhhhhhhhhhhiii~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ihh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~~~~~~~~~~ihhhhhhhhhhhhhhhhhiiii~~~~~~~~~~~~~~~i~~~~iiiiiiiiiiiihhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhiiiiiiiiiiihhhhhhhhhhhhhhhhhhhhhhiiiiiiiiiiiiiiihiiiihhhhhhhhhhhhhhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
;

const MapDescription description
{
	map_data,
	{ 81 * g_fixed16_one, 2* g_fixed16_one },
	{
		{ { { 50, 14 }, { 50, 32 } } },
		{ { { 105, 14 }, { 105, 32 } } },
		{ { { 6, 33 }, { 36, 33 } } },
	},
};

} // namespace Test

} // namespace Map05

} // namespace Armed
