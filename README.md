# ITC_Forth_in_C
Indirect threaded Forth with code in C, high level Forth in a data array, for the AVR 32u4.
Well, not really C, it's the Arduino IDE, which does things a little differently.
All the code is in the file ITC.ino. The indexes into the switch can be considered instructions in the VM.
In core.fs the same instruction numbers are compiled into Forth primitives by the target compiler.
An app needs to be compiled first in Forth by the c bash script, c for compile, which calls gforth. The target compiler makes a file named memory.h which the Arduino IDE includes when ITC.ino is compiled. Memory.h is the indirect threaded Forth. It's all data as far as the Arduino IDE is concerned.
