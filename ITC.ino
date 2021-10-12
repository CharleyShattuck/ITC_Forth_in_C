// ITC.ino  Indirect Threaded Forth

#include <Wire.h>
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

unsigned char ram[2048];

// Forth registers
unsigned int W=0; // working register
unsigned int I=0; // instruction pointer
unsigned int S=0; // data stack pointer
unsigned int R=0; // return stack pointer
unsigned int T=0; // top of stack, cached
unsigned int N=0; // next on stack, not cached

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

void i2cInit(){
    Wire.begin();
    Wire.beginTransmission(0x20);
    Wire.write(0x0c); // GPPUA
    Wire.write(0xff);
    Wire.write(0xff);
    Wire.endTransmission();
}

void i2cRead(){
    DUP;
    Wire.beginTransmission(0x20);
    Wire.write(0x12); // GPIOA
    Wire.endTransmission();
    Wire.requestFrom(0x20, 2);
    T = Wire.read();
    W = Wire.read();
    T |= W << 8;
    T ^= 0xffff;
}

void setup() {
  Serial.begin (9600);
  i2cInit();
  delay(3000);
}

void readraw() {
    DUP;
    T=digitalRead(9);
    T|=(digitalRead(10)<<1);
    T|=(digitalRead(11)<<2);
    T|=(digitalRead(12)<<3);
    T|=(digitalRead(A1)<<4);
    T|=(digitalRead(A2)<<5);
    T|=(digitalRead(A3)<<6);
    T|=(digitalRead(A4)<<7);
    T|=(digitalRead(A5)<<8);
    T^=0x01ff;
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
        rstack[R++]=I;
        I=++W;
        goto next;
    case 1: // exit
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
            I=pgm_read_word(&memory[I]);
            DROP;
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
        T|=ram[W]<<8;
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
    case 26: // @pe port expander
        i2cRead();
        goto next;
    case 27: // @pin
        W=T;
        T=digitalRead(W);
        goto next;
    case 28: // pinMode
        W=T;
        DROP;
        pinMode(W, T);
        DROP;
        goto next;
    case 29: // !pin
        W=T;
        DROP;
        digitalWrite(W, T);
        DROP;
        goto next;
    case 30: // true 
        DUP;
TRUE:   T=0xffff;
        goto next;
    case 31: // false 
        DUP;
FALSE:  T=0;
        goto next;
    case 32: // over
        W=stack[S-1];
        DUP;
        T=W;
        goto next;
    case 33: // S@
        W=S;
        DUP;
        T=W;
        goto next;
    case 34: // readraw
        readraw();
    case 35: // swap
        W=stack[S-1];
        stack[S-1]=T;
        T=W;
        goto next;
    case 36: // rot
        N=stack[S-2];
        W=stack[S-1];
        stack[S-1]=T;
        stack[S-2]=W;
        T=N;
        goto next;
    case 37: // 2*
        T=T<<1;
        goto next;
    case 38: // 0=
        if(T==0) goto TRUE;
        goto FALSE;
    case 39: // 0<
        if(T<0) goto TRUE;
        goto FALSE;
    case 40: // -
        T=T-stack[--S];
        goto next;
    case 41: // =
        T=T-stack[--S];
        if(T=0) goto FALSE;
        goto TRUE;
    case 42: // (#+)
        W=pgm_read_word(&memory[I++]);
        T+=W;
        goto next;
    default:
        // should we abort here?
        goto next;
  }  
}

