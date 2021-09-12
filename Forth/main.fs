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
warnings off
host 
: code  
   \ make a label and restore input stream
   host >in @ label >in !
   \ make a word in the host and remember current target address
   create target here 2/ host ,  \ code memory is word addressed
      \ suppress optimizations and switch to target vocab (postponed),
      hide postpone target
   \ runtime behavior is to lay down a code field in the target
   does> @ ,-t ;
: turnkey target here 2/ 0 !-t ;

target 2 org \ target-image is byte addressed here 
code lit ( c)  3 ,
:m #, ( a)  lit , m;
\ think of #, as a literal instruction in an assembler
code branch ( c)  2 ,
:m begin (  - a)  here 2/ m;
:m again ( a)  branch , m;
code emit ( c)  0 ,
code ms (  c)  1 ,
code dup ( n - n)  4 ,
code 1+ ( n - n)  5 ,
code exit  7 ,
:m :  code 6 , m;
:m ;  exit m;

: .A.  65 #, ;
turnkey ( 65 #,) .A. begin 1+ dup emit 1000 #, ms again

:m check  target-image 64 dump m;
.words check

