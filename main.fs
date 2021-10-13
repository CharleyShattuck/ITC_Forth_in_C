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
: wait  1000 #, ms ;
: read-all (  - n1 n2)  @pins @MCP23017 ;
: pressed? ( n1 n2 - n1 n2 flag)  over over or ;
: 2drop  drop drop ;
: 2or  rot or >r or r> ;
: press 
    begin read-all pressed? 0= while 2drop repeat 2drop ;
: waiting (  - n1 n2)
    begin press 30 #, ms read-all pressed? 0= while 2drop repeat ;
: accumulate ( n1 n2 - n3 n4)
    begin read-all pressed? while 2or repeat 2drop ;

: tib (  - a)  0 #, ;
: tib! ( c)  tib dup c@ 1+ over c! dup c@ + c! ;
: query
    0 #, tib ! 
    begin key BL max dup emit BL xor dup while
        BL xor tib! repeat drop ; 
: show  0 #, 10 #, for dup c@ . 1+ next drop cr ;
: test  begin query tib count type cr .s cr cr show cr again

\ : scan (  - n1 n2)
\    begin
\        begin read-all pressed? 0= while drop drop repeat
\        10 #, ms read-all pressed? 0= while  drop drop drop drop
\    repeat  rot or >r or r>
\    begin read-all pressed? while rot or >r or r> repeat
\    drop drop ;

turnkey
    wait
\    INPUT_PULLUP #,  9 #, pinMode
\    INPUT_PULLUP #, 10 #, pinMode
\    INPUT_PULLUP #, 11 #, pinMode
\    INPUT_PULLUP #, 12 #, pinMode
\    INPUT_PULLUP #, 19 #, pinMode
\    INPUT_PULLUP #, 20 #, pinMode
\    INPUT_PULLUP #, 21 #, pinMode
\    INPUT_PULLUP #, 22 #, pinMode
\    INPUT_PULLUP #, 23 #, pinMode
\    65 #, 0 #, c! show 0 #, tib !
\    key tib! key tib! key tib! show
    test
    .s cr begin again

