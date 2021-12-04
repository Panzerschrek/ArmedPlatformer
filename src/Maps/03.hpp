#pragma once
#include "../World.hpp"

namespace Armed
{

namespace Maps
{

namespace Map03
{

const char map_data[]=
"a                                                                                a\n"
"a                                                                                a\n"
"a                                                                             %  a\n"
"a                                                                           bbbbba\n"
"a                                                o                          ccccca\n"
"a                                                                                a\n"
"a                                                                    bbb         a\n"
"a              n                                                   bbccc         a\n"
"a      bbbbbbbbbbbb                  m        m    0   m         bbcc            a\n"
"a      cccccccccccab           bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  cc              a\n"
"a                 cabbbbbbb    cccccccccccccccccccccccccccccccc                  a\n"
"a                  cccccccc                                                      a\n"
"a                                                                                a\n"
"a           0                                                                    a\n"
"a      bb  bb  bbb                                                               a\n"
"a      cc  cc  ccab                                                              a\n"
"a                cab     m                       n                               a\n"
"a                 cabbbbbbbbbb      4          bbbbbbbbbbbbbb                    a\n"
"a                  ccccccccccc   bbbbbbb       cccccccccccccc   bb               a\n"
"a                                ccccccc                        aa               a\n"
"a                                           o                   aa               a\n"
"a                                                               aa               a\n"
"a                                                               aa               a\n"
"a                                                   bbbbbbbb    aa               a\n"
"a                                m                  cccccccc    aa               a\n"
"a                      4      bbbbbbbb      0                   aa               a\n"
"a                   bbbbbb    cccccccc   bbbbbb                 aa               a\n"
"a                   cccccc               cccccc                 aa               a\n"
"a           bbbbbb              o                                                a\n"
"a           cccccc                                                               a\n"
"a                                                                5               a\n"
"a                                                               bbbb             a\n"
"a              m                      n                         cccc             a\n"
"a           bbbbbbbbbbbbbb          bbbbbbb                                      a\n"
"a           cccccccccccccc          cccccaabb                                    a\n"
"a                                        ccaabb                                  a\n"
"a                                          ccaabb   0                            a\n"
"a                                            ccaabbbbbbbbbb           n          a\n"
"a                                              cccccccccccc    bbbbbbbbb         a\n"
"a                                                              aaaaaaaaa         a\n"
"a                                                              aaaaaaaaa         a\n"
"a                                                              aaaaaaaaa         a\n"
"a                                                              aaaaaaaaa         a\n"
"a                                                              aaaaaaaaa         a\n"
"a                                                    m         aaaaaaaaa         a\n"
"a                                               bbbbbbbb       ccccccccc         a\n"
"a                                 bbbbbbbbb     cccccccc                         a\n"
"a                   bbbbbbbbbb    ccccccccc                                      a\n"
"a        bbbbbbbb   cccccccccc                                                   a\n"
"a        aaaaaaaa&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&a\n"
"abbbbbbbbaaaaaaaa&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&a\n"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
;

const MapDescription description
{
	map_data,
	{ 2 * g_fixed16_one, 50 * g_fixed16_one },
	{
		{ { { 61, 46 }, { 61, 38 } } },
		{ { { 10, 34 }, { 10, 28 } } },
		{ { { 49, 27 }, { 49, 24 } } },
		{ { { 62, 32 }, { 62, 24 } } },
		{ { { 69, 32 }, { 69, 18 } } },
		{ { {  4, 15 }, {  4,  9 } } },
		{ { { 74,  6 }, { 74,  3 } } },
	},
};

} // namespace Test

} // namespace Map00

} // namespace Armed
