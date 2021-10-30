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
\ : press 
\    begin read-all pressed? 0= while 2drop repeat 2drop ;
\ : waiting (  - n1 n2)
\    begin press 30 #, ms read-all pressed? 0= while 2drop repeat ;
\ : accumulate ( n1 n2 - n3 n4)
\    begin read-all pressed? while 2or repeat 2drop ;

here [ 4 + constant dict ]
: dictionary  $a5 #, p! ;
: tib (  - a)  0 #, ;
: tib! ( c)  tib dup c@ 1+ over c! dup c@ + c! ;
: echo ( c - c)  dup emit ;
: query
    0 #, tib ! false
    begin drop key BL max BL xor until BL xor echo tib!
    begin key BL max BL xor while BL xor echo tib! repeat
    drop BL tib dup c@ + 1+ c! ;
: match (  - 0|n)  \ P has been loaded
    tib a! false p @p $ff #, and 2/ for @+ @p+ - or next
    @p+ swap if 2drop false exit then drop ; 
: find (  - a|0)
    dictionary  \ loads P register
    begin p @p while drop
        tib a! match if exit then drop
    repeat ;
: interpret
    begin
        begin .sh cr query space find while execute repeat
        tib count type huh?
    again
: digit ( n1 - n2)  $3a #, - -if 10 #, + exit then 29 #, - ; 
: h# (  - n)  0 #,
    begin key BL max BL xor while
        BL xor digit swap 2* 2* 2* 2* or
    repeat drop ; 

: test ( n - )  -if true . exit then false . ;

$ffff , $ffff ,
: this char A #, emit char B #, emit char C #, emit cr ;
$ffff , $ffff ,
: one  1 #, ;
: 'A'  $41 #, ;
\ : scan (  - n1 n2)
\    begin
\        begin read-all pressed? 0= while drop drop repeat
\        10 #, ms read-all pressed? 0= while  drop drop drop drop
\    repeat  rot or >r or r>
\    begin read-all pressed? while rot or >r or r> repeat
\    drop drop ;
: down  10 #, begin dup . 1- -until drop ;

turnkey wait interpret

