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
turnkey
    wait 
    INPUT_PULLUP #,  9 #, pinMode
    INPUT_PULLUP #, 10 #, pinMode
    INPUT_PULLUP #, 11 #, pinMode
    INPUT_PULLUP #, 12 #, pinMode
    INPUT_PULLUP #, 19 #, pinMode
    INPUT_PULLUP #, 20 #, pinMode
    INPUT_PULLUP #, 21 #, pinMode
    INPUT_PULLUP #, 22 #, pinMode
    INPUT_PULLUP #, 23 #, pinMode
\    0 #, for r@ . next cr
    begin
\        5 #, for  r@ . next cr .s cr
        @pe .
        9 #, @pin . 10 #, @pin . 11 #, @pin . 12 #, @pin . 
        19 #, @pin . 20 #, @pin . 21 #, @pin . 22 #, @pin . 
        23 #, @pin . cr wait
    again

