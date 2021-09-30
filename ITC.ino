// ITC.ino  Indirect Threaded Forth

#include "memory.h"
// #include <Keyboard.h>  // HID
// #include <Wire.h>      // i2c
// it seems that Serial.h is loaded by default

// stack grows upwards, makes .s easier
const int STKSIZE=16;
int stack[STKSIZE];
#define DROP T=stack[--S]
#define DUP stack[S++]=T
const int RSTKSIZE=16;
int rstack[RSTKSIZE];
#define PUSH rstack[R++]=T
#define POP T=rstack[--R]

// unsigned char ram[1024];
unsigned char ram[1024];

// Forth registers
int W=0; // working register
int I=0; // instruction pointer
int S=0; // data stack pointer
int R=0; // return stack pointer
int T=0; // top of stack, cached be

void dotS () {
    switch(S) {
    case 0:
        Serial.print("empty ");
        return;
    case 1:
        Serial.print(T, HEX);
        Serial.print(' ');
        return;
    default:
        for (int i=1; i<S; i++) {
            Serial.print(stack[i], HEX);
            Serial.print(' ');
        }
        Serial.print(T, HEX);
        Serial.print(' ');
    }
}
 
void setup() {
  Serial.begin (9600);
}

// code words all in one function
// so we can avoid calls and just jump to next
void loop () {
  I=pgm_read_word(&memory[0]);
next: 
  W=pgm_read_word(&memory[I++]);
ex:
  switch (pgm_read_word(&memory[W])) {
    case 0: // enter
//        --R=I;
        rstack[R++]=I;
        I=++W;
        goto next;
    case 1: // exit
//        I=R++;
        I=rstack[--R];
        goto next;
    case 2: // emit
        Serial.write(T);
        DROP;
        goto next;
    case 3: // ms
        delay(T);
        DROP;
        goto next;
    case 4: // branch
        I=pgm_read_word(&memory[I]);
        goto next;
    case 5: // 0branch
        if(T==0) {
            DROP;
            I=pgm_read_word(&memory[I]);
            goto next;
        }
        I+=1;
        DROP;
        goto next;
    case 6: // lit
        DUP;
        T=pgm_read_word(&memory[I++]);
        goto next;
    case 7: // .
        Serial.print(T, HEX);
        Serial.print(' ');
        DROP;
        goto next;
    case 8: // .s
        dotS();
        goto next;
    case 9: // dup
        DUP;
        goto next;
    case 10: // drop
        DROP;
        goto next;
    case 11: // +
        T=T+stack[--S];
        goto next;
    case 12: // and
        T=T&stack[--S];
        goto next;
    case 13: // or
        T=T|stack[--S];
        goto next;
    case 14: // xor
        T=T^stack[--S];
        goto next;
    case 15: // key
        DUP; 
        while (!Serial.available());
        T=Serial.read();
        goto next;
    case 16: // execute
        W=pgm_read_word(&memory[T]);
        DROP;
        goto ex;
    case 17: // @p
        W=T;
        T=pgm_read_word(&memory[W]);
        goto next;
    case 18: // c@
        W=T;
        T=ram[W];
        goto next;
    case 19: // @
        W=T;
        T=ram[W++];
        T|=ram[W];
        goto next;
    case 20: // c!
        W=T;
        DROP;
        ram[W]=T;
        DROP;
        goto next;
    case 21: // !
        W=T;
        DROP;
        ram[W++]=T&0xff;
        ram[W]=(T>>8)&0xff;
        DROP;
        goto next;
    case 22: // >r
        PUSH;
        DROP;
        goto next;
    case 23: // r>
        DUP;
        POP;
        goto next;
    case 24: // r@
        DUP;
        T=rstack[R-1];
        goto next;
    case 25: // next
        W=rstack[R-1];
        if(W) {
            rstack[R-1]=W-1;
            I=pgm_read_word(&memory[I]);
            goto next;
        }
        R-=1;
        I+=1;
        goto next;
    default:
        // should we abort here?
        goto next;
  }  
}

