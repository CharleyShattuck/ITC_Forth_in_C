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

\ steno keyboard app
: 2drop  drop drop ;
: 2or ( n1 n2 n3 n4 - n5 n6)  rot or >r or r> ;
: wait  1000 #, ms ;
: keys (  - n1 n2)  @pins @i2c ;
: pressed? ( n1 n2 - n1 n2 flag)  over over or ;
: press  begin keys pressed? 0= while drop 2drop repeat drop 2drop ;
: waiting (  - n1 n2)
    begin press 10 #, ms keys pressed? 0= while drop 2drop repeat drop ;
: accumulate ( n1 n2 - n3 n4)
    begin keys pressed? while drop 2or repeat drop 2drop ;
: scan (  - n1 n2)  waiting accumulate ;
cvariable first
\ : spit ( c)
\    dup Keyboard.press 2 #, ms
\    first c@ if 0= first c!  exit then
\    drop Keyboard.release ;
: spit ( c)  false first c! emit 2 #, ms ;
0 [if]
: NKRO ( n1 n2 - )
    true first c!
    over  $10 #, and if [ char q ] #, spit then drop
    over  $20 #, and if [ char w ] #, spit then drop
    over  $40 #, and if [ char e ] #, spit then drop
    over  $80 #, and if [ char r ] #, spit then drop
    over $100 #, and if [ char t ] #, spit then drop
    dup $8000 #, and if [ char u ] #, spit then drop
    dup $4000 #, and if [ char i ] #, spit then drop
    dup $2000 #, and if [ char o ] #, spit then drop
    dup $1000 #, and if [ char p ] #, spit then drop
    dup  $100 #, and if [ char [ ] #, spit then drop
    over  $08 #, and if [ char a ] #, spit then drop
    over  $04 #, and if [ char s ] #, spit then drop
    over  $02 #, and if [ char d ] #, spit then drop
    over  $01 #, and if [ char f ] #, spit then drop
    dup  $200 #, and if [ char g ] #, spit then drop
    dup   $01 #, and if [ char j ] #, spit then drop
    dup   $02 #, and if [ char k ] #, spit then drop
    dup   $04 #, and if [ char l ] #, spit then drop
    dup  $800 #, and if [ char ; ] #, spit then drop
    dup  $400 #, and if [ char \ ] #, spit then drop
    dup   $08 #, and if [ char c ] #, spit then drop
    dup   $10 #, and if [ char v ] #, spit then drop
    dup   $20 #, and if [ char 3 ] #, spit then drop
    dup   $40 #, and if [ char n ] #, spit then drop
    dup   $80 #, and if [ char m ] #, spit then drop
    2drop ;
[then]
: Gemini ( n1 n2 - )
    $80 #, >r
    over $10 #, and if r> $40 #, or >r then drop \ S1
    over $08 #, and if r> $20 #, or >r then drop \ S2
    over $20 #, and if r> $10 #, or >r then drop \ T
    over $04 #, and if r> $08 #, or >r then drop \ K
    over $40 #, and if r> $04 #, or >r then drop \ P
    over $02 #, and if r> $02 #, or >r then drop \ W
    over $80 #, and if r> $01 #, or >r then drop \ H
    r> h. 0 #, >r 
    dup   $01 #, and if r> $40 #, or >r then drop \ R
    dup   $08 #, and if r> $20 #, or >r then drop \ A
    dup   $10 #, and if r> $10 #, or >r then drop \ O
    over $100 #, and if r> $08 #, or >r then drop \ *
    dup  $200 #, and if r> $04 #, or >r then drop \ *
    r> h. 0 #, >r
    dup   $40 #, and if r> $08 #, or >r then drop \ E
    dup   $80 #, and if r> $04 #, or >r then drop \ U
    dup $8000 #, and if r> $02 #, or >r then drop \ F
    dup   $01 #, and if r> $01 #, or >r then drop \ R
    r> h. 0 #, >r
    dup $4000 #, and if r> $40 #, or >r then drop \ P
    dup   $02 #, and if r> $20 #, or >r then drop \ B
    dup $2000 #, and if r> $10 #, or >r then drop \ L
    dup   $04 #, and if r> $08 #, or >r then drop \ G
    dup $1000 #, and if r> $04 #, or >r then drop \ T
    dup  $800 #, and if r> $02 #, or >r then drop \ S
    dup  $100 #, and if r> $01 #, or >r then drop \ D
    r> h. 0 #, >r
    dup   $20 #, and if r> $40 #, or >r then drop \ #7
    dup  $400 #, and if r> $01 #, or >r then drop \ Z
    r> h.
    drop drop cr ;

: test   begin scan Gemini .sh cr again

0 [if]
: .pin  \ use to see which key is which pin
    false begin drop @pins until h.
    begin @pins while drop repeat drop cr ;
: .i2c  \ same but for the port expander
    false begin drop @i2c until h.
    begin @i2c while drop repeat drop cr ;
[then]

turnkey 1000 #, ms interpret

