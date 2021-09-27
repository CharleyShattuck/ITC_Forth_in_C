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
    10000 #, ms 0 #, 
    10 #, 0 #, c!
    9 #, 1 #, c!
    8 #, 2 #, c!
    7 #, 3 #, c!
    6 #, 4 #, c!
    5 #, 5 #, c!
    4 #, 6 #, c!
    3 #, 7 #, c!
    2 #, 8 #, c!
    1 #, 9 #, c!
    0 #, 10 #, c!
    begin dup . dup c@ . 1 #, + cr wait again

