\ main.fs 

0 [if]
Copyright (C) 2016-2021 by Charles Shattuck.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

For LGPL information:   http://www.gnu.org/copyleft/lesser.txt

[then]
\ warnings off
\ variables
\ : K1  [ cpuHERE #, 2 cpuALLOT ] ;
\ : K2  [ cpuHERE #, 2 cpuALLOT ] ;
variable K1
variable K2
: 2drop  drop drop ;
: 2or ( n1 n2 n3 n4 - n5 n6)  rot or >r or r> ;
: wait  1000 #, ms ;
: keys (  - n1 n2)  @pins @i2c ;
: pressed? ( n1 n2 - n1 n2 flag)  over over or ;
: press  begin keys pressed? 0= while drop 2drop repeat drop 2drop ;
: waiting (  - n1 n2)
    begin press 20 #, ms keys pressed? 0= while drop 2drop repeat drop ;
: accumulate ( n1 n2 - n3 n4)
    begin keys pressed? while drop 2or repeat drop 2drop ;
: scan  waiting accumulate K2 ! K1 ! ;
: first  [ cpuHERE #, 1 cpuALLOT ] ;
: NKRO ( n1 n2 - )
    false first c! 
    ;

0 [if]
: .pin  \ use to see which key is which pin
    false begin drop @pins until h.
    begin @pins while drop repeat drop cr ;
: .i2c  \ same but for the port expander
    false begin drop @i2c until h.
    begin @i2c while drop repeat drop cr ;
[then]

turnkey wait interpret

