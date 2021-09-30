\ core.fs 

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
: -code  
   \ make a label and restore input stream
   host >in @ label >in !
   \ make a word in the host and remember current target address
   create target here 2/ host ,  \ code memory is word addressed
      \ suppress optimizations and switch to target vocab (postponed),
      hide postpone target
   \ runtime behavior is to lay down a code field in the target
   does> @ ,-t ;
: code  -code header ;
: turnkey target here 2/ 0 !-t ;

target 2 org \ target-image is byte addressed here on the Forth side
code exit  1 ,
code emit  2 ,
code ms  3 ,
-code branch  4 ,
-code 0branch  5 ,
-code lit ( c)  6 ,
code . ( n)  7 ,
code .s  8 ,
code dup ( n - n)  9 ,
code drop ( n)  10 ,
code + ( n1 n2 - n3)  11 ,
code and ( n1 n2 - n3)  12 ,
code or ( n1 n2 - n3)  13 ,
code xor ( n1 n2 - n3)  14 ,
code key (  - c)  15 ,
code execute ( a)  16 ,
code @p ( a - n)  17 ,
code c@ ( a - c)  18 ,
code @ ( a - c)  19 ,
code c! ( c a - )  20 ,
code ! ( n a - )  21 ,
code >r ( n - )  22 ,
code r> (  - n)  23 ,
code r@ (  - n)  24 ,
-code (next)  25 ,

\ think of #, as a literal instruction in an assembler
:m #, ( a)  lit , m;
:m begin (  - a)  here m;
:m again ( a)  branch [ 2/ ] , m;
:m until ( a)  0branch [ 2/ ] , m;
:m then ( a)  here [ 2/ swap ] !-t ;
:m if ( - a)  0branch begin 0 , m;
:m while ( a1 - a2 a1)  if [ swap ] m;
:m repeat ( a1 a2 - )  again then m;
:m for  >r begin m;
:m next  (next) [ 2/ ] , m;
:m -:  -code  0 , m;
:m :  -: header m;
:m ;  exit m;

: space  32 #, emit ;
: cr  13 #, emit 10 #, emit ;

