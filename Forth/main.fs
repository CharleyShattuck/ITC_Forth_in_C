\ main.fs 

0 [if]
Copyright (C) 2016-2018 by Charles Shattuck.

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
target

0 org  
7 ,
code lit ( c)  3 ,
code branch ( c)  2 ,
code emit ( c)  0 ,
code ms (  c)  1 ,
code dup ( n - n)  4 ,
code 1+ ( n - n)  5 ,

lit 65 , ( begin) 1+ dup emit lit 1000 , ms branch 9 , ( again)

:m check  target-image 64 dump m;
check .words

