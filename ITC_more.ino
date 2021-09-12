// ITC.ino  Indirect Threaded Forth

// #include <Keyboard.h>  // HID
// #include <Wire.h>      // i2c
// it seems that Serial.h is loaded by default

// circular stack, similar to GA144
const int STKSIZE = 8;
const int STKMASK = 7;
int stack [STKSIZE];
#define DROP T=stack[S++];S&=STKMASK
#define PUSH S=--S&STKMASK
#define DUP PUSH;stack[S]=T

// Forth registers
int W=0; // working register
int I=0; // instruction pointer
int S=0; // data stack pointer
int R=0; // return stack pointer
int T=0; // top of stack, cached

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
    case 0: // emit
        Serial.write(T);
        DROP;
        goto next;
    case 1: // ms
        delay(T);
        DROP;
        goto next;
    case 2: // branch
        I=pgm_read_word(&memory[I++]);
        goto next;
    case 3: // lit
        PUSH;
        T=pgm_read_word(&memory[I++]);
        goto next;
    case 4: // dup
        DUP;
        goto next;
    case 5: // 1+
        T=T+1;
        goto next;
    case 6: // enter
        R=--R&STKMASK;
        R=I;
        I=++W;
        goto next;
    case 7: // exit
        I=R;
        R=++R&STKMASK;
        goto next;
    default:
        goto next;
  }  
}
