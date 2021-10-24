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
code @MCP23017 (  - n)  26 ,
code @pin (  - n)  27 ,
code pinMode ( mode pin - )  28 ,
code !pin ( value pin - )  29 ,
code true (  - $ffff)  30 ,
code false (  - 0)  31 ,
code over ( a b - a b a)  32 ,
code S@ (  - n)  33 ,
code @pins (  - n)  34 ,
code swap ( a b - b a)  35 ,
code rot ( a b c - b c a)  36 ,
code 2* ( n1 - n2)  37 ,
code 0= ( n - flag)  38 ,
code 0< ( n - flag)  39 ,
code - ( n1 n2 - n3)  40 ,
code = ( n1 n2 - flag)  41 ,
code (#+) ( n1 - n2)  42 ,
code < ( a b - c)  43 ,
code p! ( a - )  44 ,
code p (  - a)  45 ,
code @p+ (  - a)  46 ,
code @+ (  - a)  47 ,
code c@+ (  - c)  48 ,
code a! ( a - )  49 ,
code a  (  - a)  50 ,
code 2/ ( n1 - n2)  51 ,
code cr (  - )  52 ,
\ only makes sense in the interpreter
code d# (  - n)  53 ,
code abort  54 ,
code quit  55 ,
code +branch  56 ,

0 constant INPUT
1 constant OUTPUT
2 constant INPUT_PULLUP
\ HIGH
\ LOW

\ think of #, as a literal instruction in an assembler
:m #, ( a)  lit , m;
:m #+ ( n1 - n2)  (#+) , m;
:m 1+ ( n1 - n2)  1 #+ m;
:m begin (  - a)  here m;
:m again ( a)  branch [ 2/ ] , m;
\ 0branch doesn't drop the stack
:m until ( a)  0branch [ 2/ ] , m;
:m -until ( a)  +branch [ 2/ ] , m;
:m then ( a)  here [ 2/ swap ] !-t ;
:m if (  - a)  0branch begin 0 , m;
:m -if (  - a)  +branch begin 0 , m;
:m while ( a1 - a2 a1)  if [ swap ] m;
:m -while ( a1 - a2 a3)  -if [ swap ] m;
:m repeat ( a1 a2 - )  again then m;
:m for  >r begin m;
:m next  (next) [ 2/ ] , m;
:m -:  -code  0 , m;
:m :  code  0 , m;
:m ;  exit m;

: space  32 #, emit ;
\ : cr  13 #, emit 10 #, emit ;
: count ( a1 - a2 c)  dup 1+ swap c@ ;
: type ( a l - )  -1 #+ for
        dup c@ emit 1+
    next drop ;
: BL  32 #, ;
: max ( a b - c)  over over < if
        drop swap drop exit
    then drop drop ;
: 1+  1 #+ ;
: 1-  -1 #+ ;

