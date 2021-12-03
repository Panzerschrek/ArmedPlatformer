#pragma once
#include "../World.hpp"

namespace Armed
{

namespace Maps
{

namespace Map02
{

const char map_data[]=
"                                                                                                                        \n"
"                        bbbbbbbbbbbbbbbbbbbbbbbbbbb                        bbbbbbbbbbbbbb                               \n"
"         bbbbbbbbbbbbbbbaaaccccccccccccccccccccccaabbbb                    cacccccaaaaaaa                               \n"
"b       baaaaaaaaaaaaaaaaaa                      caaaac                     a     ccaaaaabb                            b\n"
"a       aaaaaaaaaaaaaaaaaaa                       caac                      B       aaaaaaabbbb                        a\n"
"a       aaaaaaaaaaaaaaaaaaa:   m       m           cc         5             B       aaaaaaaaaaab                       a\n"
"abbbb  baaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa        bbbbbbbbbb     bbbbbabb     aaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbba\n"
"aaaaa  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa      baaaaaaaaaab     caaaaaab     aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"aaaaa  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbaaaaaaaaaaaab     caaaaaabb    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
"hhhhh  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhi     jhhhhhhhi    jhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhh  hhjjjjjjjjjjjhhhjjjjjjjjjj          hhh        h         hhhhhi     jjjhhhhhi    jhhhhhhjjjjhhhhhhhhhhhhhhhhhhhhh\n"
"hhhhh  hh           jjj                    hhh        h         hhhhhhi       jhhhhhi    jjjjjj    jhhhhhhhhhhhhhhhhhhhh\n"
"hhhhh  hh                                  hhh        A         hhhhhhhi       hhhhhhi              jjjhhhhhhhhhhhhhhhhh\n"
"hhhhh  hh           m               n    8 hhh        A   n     hhhhhhhhhh     hhhhhhhi     0          jhhhhhhhhhhhhhhhh\n"
"hhhhh~~hh~~iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiihhh~~iiiiiihiiiiiii~~hhhhhhh~~~~~~iihhhhhhhhiiiiiiiiii       hhhhhhhhhhhhhhhh\n"
"hhhhh~~hh~~hhhhhhhhhhhhhhhhhhhhhhhhhhhj jhhhhh~~hhhhhhhhhhhhhh~~hhhhhhh~~~~~hhhhhhhhhhhhhhhhhhhhhiii    hhhhhhhhhhhhhhhh\n"
"hhhhh~~hh~~jjhhhhhhhhhhhhhhhhhhhhhhhjj   jhhhh~~jhhhhhhhhhhhhh~~hhhhhhh~~iiihhhhhhhhhhhhhhhhhhhhhhhhi  ihhhhhhhhhhhhhhhh\n"
"hhhhh~~hh~~~~jjjjjjjjjjjhhhjjjjjjjjj~~~~~jjjjj~~~jjhhhhhhhhhhh~~hhhhhhh~~hhhhhhhhhhhhhjjjhhhhhhhhhhhh  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hh~~~~~~~~~~~~~~~jjj~~~~~~~~~~~~~~~~~~~~~~~~hhhhhhhhhh~~~hhhhhhh~~~~hhhhhhhhhhh~~~jhhhhhhhhhhh  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hh~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hhhhhhhhhh~~~hhhhhhh~~~~hhhhhhhhhjjh~~~hhhhhhhhhhh  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhiiii~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ihhhhhhhhhh~~ihhhhhhhii~~hhhhhhhhh~~~~~~hhhhhhhhhhh  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhhhhiiiiiiiiii~~~~iiiiiiiiii~~iiii~~~iiihhhhhhhhhhhh~~jhhhhhhhhh~~hhhhhhhhh~~~~~~hhhhhhhhhhh  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhhhhhhhhhhhhhhiiiihhhhhhhhhh~~hhhhiiihhhhhhhhhjjjjjj~~~jjjhhhhjj~~jjhhhhhhjh~~~~hhhhhhhhhhhh  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhhjjjjjjjjjjjjjjjjjjhhhhhhhh~~hhhhhhhhhhhhjjjj~~~~~~~~~~~~jjjj~~~~~~hhhhhj~~~~~~~hhhhhhhhhhh  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhh~~~~~~~~~~~~~~~~~~~~jjjjjj~~hhhhhhhhhhjj~~~~~~~~~~~~~~~~~~~~~~~~~~hhhhh~~~~~~~~~hhhhhhhhhj  hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhh~~~~~~~~~~~~~~~~~~~~~~~~~~~~~hhhhhhhhh~~~~~~~~~~~~iiii~~~~~~~~~~~~hhhhhiiiiiiiiihhjjjjjjj   hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhh~~~iii~~~~~~~~~~~~~~~~~~~~~~ihhhhhhhhh~~~~~~~iiiiihhhhii~~~~iiiiiihhhjjjjjjjjjjjjj          hhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhh~~~jjhiiiiiiiiiiiiiiiiiiiiiihhhhhhhhhhi~~iiiihhhhhhhhhhhiiiihhhhhhhhh                      ihhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhhi~~~~jjhhhhhhhhh   hhhhhhhhhhhhhhhhhhhh~~hhhhhhhhhhhhhhhhhhhhhhhhhhjj    m              m  hhhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhhh~~~~~~jjjjjjjjj~~hhhhhhhhhhhhhhhhhhhhj~~hhhhhhhhhhhhhhhhhhhhhhhhhh     iiiiiiiiiiiiiiiiiihhhhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhhhii~~~~~~~~~~~~~~~jhhhh  h      hhhhhh~~~hhhhhhhhhhhhhhhhhhhhhhhhhj    ihhhhhhhhhhhhjjjjjjjjjhhhhhhhhhhhhhhhh\n"
"hhhhh~~hhhhhhhii~~~~~~~~~~~~~~hhhh  h      hhhhhh~~~hhhhhhhhhhhhhhhhhhhhhhhhj    ihhhhhhhhhhhhj         hhhhhhhhhhhhhhhh\n"
"hhhhj~~jjjhhhhhhi~~~~~~~~~~~~~hhhh  A      hhhhhh~~~hhhhhhhhhhhhhhhhhhhhhhhj    ihhhhhhhhhhhhj          jhhhhhhhhhhhhhhh\n"
"hhjj~~~~~~jjhhhhhiiiiiiiiii~~hhhhh9 A      hhhhhh~~ihhhhjjjjjjjjjjhjjjjjjjj    ihhhhhhhhhhhhj    iii     jhhhhhhhhhhhhhh\n"
"hh~~~~~~~~~~jjjjjjjjjjjjjjj~~hhhhhiihiiii~~hhhhhh~~hhhhh          C           ihhhhhhhhhhhhj    ihhhi     hhhhhhhhhhhhhh\n"
"hh~~~~~~~~~~~~~~~~~~~~~~~~~~~hhhhhhhhhhhh~~jhhhhj~~hhhhh    m     C          ihhhhhhhhhhhhj    ihhhhhi    jjhhhhhhhhhhhh\n"
"hh~~~~~~~~~~~~~~~~~~~~~~~~~~~hhhhhhhhhhhh~~~jjjj~~~hhhhh~~iiiiiiiihiiiiiiiiiihjjjhhhhjjhhj    ihhhhhhhi              hhh\n"
"hhii~~~ii~~~~~~~~~~~~~~~~~~~ihhhhhhhhhhhh~~~~~~~~~~hhhhh~~jjjjjjjj~jjjjjjjjjjj~~~jjjj  jj~~~~ihhhhhhhhhi             hhh\n"
"hhhhiiihhiiiiiiiiiiiiiiiiiihhhhhhhhhhhhhh~~~~~~~~~~hhhhh~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ihhhhhhhhhhhi            hhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh~~~~~~~~~~hhhhh~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ihhhhhhhhhhhhhi     n   % hhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhiiiiiiiiiihhhhhiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiihhhhhhhhhhhhhhhhiiiiiiiiiihhh\n"
"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"

;

const MapDescription description
{
	map_data,
	{ 2 * g_fixed16_one, 5 * g_fixed16_one },
	{
	},
};

} // namespace Test

} // namespace Map01

} // namespace Armed
