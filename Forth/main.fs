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
code exit  1 ,
code emit  2 ,
code ms  3 ,
code branch  4 ,
code 0branch  5 ,
code lit ( c)  6 ,
code . ( n)  7 ,
code .s  8 ,
code dup ( n - n)  9 ,
code drop ( n)  10 ,
code 1+ ( n - n+1)  11 ,
\ think of #, as a literal instruction in an assembler
:m #, ( a)  lit , m;
:m begin (  - a)  here 2/ m;
:m again ( a)  branch , m;
:m until ( a)  0branch , m;
:m if ( - a)  0branch begin 0 , m;
:m then ( a)  here swap !-t ;
:m :  code  0 , m;
:m ;  exit m;

: space  32 #, emit ;
: cr  13 #, emit 10 #, emit ;
turnkey
\    begin .s cr 1 #, .s cr 2 #, .s cr 3 #, .s cr
\    drop drop drop .s cr 1000 #, ms cr again
  1 #, 2 #, 3 #, 65535 #, char A #, begin  .s cr 
  dup . dup emit space 1000 #, ms cr again

:m check  target-image 128 dump m;
.words check

