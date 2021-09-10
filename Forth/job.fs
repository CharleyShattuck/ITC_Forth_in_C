\ job.fs

0 [if]
Copyright (C) 2009-2021 by Charles Shattuck.

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

only forth also definitions
: nowarn warnings off ; : warn warnings on ; : not 0= ;
nowarn

\ Colors are used by the decompiler/disassembler
include ansi.fs  \ Part of Gforth.
warn
variable colors
: in-color  true colors ! ;
in-color
: b/w  false colors ! ;
: color  ( n - ) create , does> colors @ if @ >fg attr! exit then drop ;
red color >red
black color >black
blue color >blue
green color >green
cyan color >cyan
yellow color >yellow

\ For navigating the source code in the VIM editor
include ./vtags.fs use-tags

0 constant start  \ Reset vector.
$8000 constant target-size

include ./compiler.fs
include ./main.fs  \ application code, ends with go

host : .stack  depth if  >red  then  .s >black cr ;
save host .( Host stack= ) .stack

