// ITC.ino  Indirect Threaded Forth

// #include <Keyboard.h>  // HID
// #include <Wire.h>      // i2c
// it seems that Serial.h is loaded by default

// circular stack, similar to GA144
const int STKSIZE = 16;
const int STKMASK = 15;
int stack [STKSIZE];

// Forth registers
int W=0; // working register
int I=0; // instruction pointer
int S=0; // data stack pointer
int R=0; // return stack pointer

void setup() {
  Serial.begin (9600);
}

// code words all in one function
// so we can avoid calls and just jump to next
void loop () {
  I=pgm_read_word(&memory[0]);
next: 
  W=pgm_read_word(&memory[I++]);
  switch (pgm_read_word(&memory[W])) {
    case 0: Serial.write(stack[S++]);goto next; // emit
    case 1: delay(stack[S++]);goto next;        // ms
    case 2: I=pgm_read_word(&memory[I++]);goto next; // branch
    case 3: stack[--S]=pgm_read_word(&memory[I++]);goto next; // lit
    default: goto next;
  }  
}
