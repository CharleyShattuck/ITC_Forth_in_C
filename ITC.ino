// ITC.ino  Indirect Threaded Forth

// #include <Wire.h>
#include "memory.h"
// #include <Keyboard.h>  // HID
// #include <Wire.h>      // i2c
// it seems that Serial.h is loaded by default

// stack grows upwards, makes .s easier
const u16 STKSIZE=16;
u16 stack[STKSIZE];
#define DROP T=stack[--S]
#define DUP stack[S++]=T
const u16 RSTKSIZE=16;
u16 rstack[RSTKSIZE];
#define PUSH rstack[R++]=T
#define POP T=rstack[--R]

u8 ram[2048];

// Forth registers
u16 W=0; // working register
u16 I=0; // instruction pointer
u16 S=0; // data stack pointer
u16 R=0; // return stack pointer
u16 T=0; // top of stack, cached
u16 N=0; // next on stack, not cached
u16 A=0; // RAM address register
u16 P=0; // program address register

void dotS () {
    switch(S) {
    case 0:
        Serial.print("empty ");
        return;
    case 1:
        Serial.print(T);
        Serial.print(' ');
        return;
    default:
        for (int i=1; i<S; i++) {
            Serial.print(stack[i]);
            Serial.print(' ');
        }
        Serial.print(T);
        Serial.print(' ');
    }
}

/*
void i2cInit(){
    Wire.begin();
    Wire.beginTransmission(0x20);
    Wire.write(0x0c); // GPPUA
    Wire.write(0xff);
    Wire.write(0xff);
    Wire.endTransmission();
}
*/

/*
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
*/

void setup() {
  Serial.begin (9600);
//  i2cInit();
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
abort:
  S=0;
quit:
  R=0;
  I=pgm_read_word(&memory[0]);
next:
  W=pgm_read_word(&memory[I++]);
ex:
  switch (pgm_read_word(&memory[W++])) {
    case 0: // enter
        rstack[R++]=I;
        I=W;
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
            goto next;
        }
        I+=1;
        goto next;
    case 6: // lit
        DUP;
        T=pgm_read_word(&memory[I++]);
        goto next;
    case 7: // .
        Serial.print(T);
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
//        Serial.print("I= ");
//        Serial.println(I);
//        Serial.print("T= ");
//        Serial.println(T);
        if(T==0) goto abort;
//        W=pgm_read_word(&memory[T]);
        W=T;
//        Serial.print("W= ");
//        Serial.println(W);
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
//        i2cRead();
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
        goto next;
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
        if(T==0) goto FALSE;
        goto TRUE;
    case 42: // (#+)
        W=pgm_read_word(&memory[I++]);
        T+=W;
        goto next;
    case 43: // <
        W=T;
        DROP;
        if(T<W) goto TRUE;
        goto FALSE;
    case 44: // p!
        P=T;
        DROP;
        goto next;
    case 45: // p
        DUP;
        T=P;
        goto next;
    case 46: // @p+
        DUP;
        T=pgm_read_word(&memory[P++]);
        goto next;
    case 47: // @+
        DUP;
        T=ram[A++];
        T|=ram[A++]<<8;
        goto next;
    case 48: // c@+
        DUP;
        T=ram[A++];
        goto next;
    case 49: // a!
        A=T;
        DROP;
        goto next;
    case 50: // a
        DUP;
        T=A;
        goto next;
    case 51: // 2/
        T=T/2;
        goto next;
    case 52: // cr
        Serial.write("\n");
        goto next;
    case 53: // d#
        DUP;
        T=Serial.parseInt(SKIP_WHITESPACE);
        goto next;
    case 54: // huh?
        Serial.write(" ?\n");
// falls through into abort
    case 55: // abort
        goto abort;
    case 56: // quit
        goto quit;
    case 57: // +branch
        W=T;
        if(W&0x8000){
            I+=1;
            goto next;
        }
        I=pgm_read_word(&memory[I]);
        goto next;
    case 58: // nip
        S-=1;
        goto next;
    case 59: // invert
        T=T^0xffff;
        goto next;
    default:
        goto abort;
  }  
}

