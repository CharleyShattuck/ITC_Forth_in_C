// ITC.ino  Indirect Threaded Forth

#include "memory.h"
// #include <Keyboard.h>  // HID
#include <Wire.h>      // i2c
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

u8 ram[1024];

// Forth registers
u16 W=0; // working register
u16 I=0; // instruction pointer
u16 S=0; // data stack pointer
u16 R=0; // return stack pointer
u16 T=0; // top of stack, cached, signed
u16 N=0; // next on stack, not cached
u16 A=0; // RAM address register
u16 P=0; // program address register
unsigned long elapsed=0;

void _dots() {
    switch(S) {
    case 0:
        Serial.print(" empty ");
        return;
    case 1:
        Serial.print(T);
        Serial.print(' ');
        return;
    default:
        for (int i=1; i<S; i++) {
            W=stack[i];
            Serial.print(W);
            Serial.print(' ');
        }
        Serial.print(T);
        Serial.print(' ');
    }
}

void _dotsh () {
    Serial.print(" ");
    switch(S) {
    case 0:
        Serial.print("empty ");
        return;
    case 1:
        Serial.print(T, HEX);
        Serial.print(' ');
        return;
    default:
        for (u16 i=1; i<S; i++){
//            W=stack[i];
//            Serial.print(W, HEX);
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


void _i2cread(){
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

void _fetchpins() {
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

void _initPins() {
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP);
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
}

void setup() {
  Serial.begin (9600);
  i2cInit();
  _initPins();
//  Keyboard.begin();
  delay(3000);
}

void _enter(){
    rstack[R++]=I;
    I=W;
}

void _exit(){
    I=rstack[--R];
}

void _emit(){
    Serial.write(T);
    DROP;
}

void _ms(){
    delay(T);
    DROP;
}

void _branch(){
    I=pgm_read_word(&memory[I]);
}

void _0branch(){
    if(T==0) {
        I=pgm_read_word(&memory[I]);
        return;
    }
    I+=1;
}

void _lit(){
    DUP;
    T=pgm_read_word(&memory[I++]);
}

void _dot(){
    Serial.print(T);
    Serial.print(' ');
    DROP;
}

void _dup(){
    DUP;
}

void _drop(){
    DROP;
}

void _plus(){
    T=T+stack[--S];
}

void _and(){
    T=T&stack[--S];
}

void _or(){
    T=T|stack[--S];
}

void _xor(){
    T=T^stack[--S];
}

void _key(){
    DUP; 
    while (!Serial.available());
    T=Serial.read();
}

// void _execute(){
//    W=T;
//    DROP;
// }

void _fetchp(){
    W=T;
    T=pgm_read_word(&memory[W]);
}

void _cfetch(){
    W=T;
    T=ram[W];
}

void _fetch(){
    W=T;
    T=ram[W++];
    T|=ram[W]<<8;
}

void _cstore(){
    W=T;
    DROP;
    ram[W]=T;
    DROP;
}

void _store(){
    W=T;
    DROP;
    ram[W++]=T&0xff;
    ram[W]=(T>>8)&0xff;
    DROP;
}

void _next(){
    W=rstack[R-1];
    if(W) {
        rstack[R-1]=W-1;
        I=pgm_read_word(&memory[I]);
        return;
    }
    R-=1;
    I+=1;
}

void _tor(){
    PUSH;
    DROP;
}

void _rfrom(){
    DUP;
    POP;
}

void _rfetch(){
    DUP;
    T=rstack[R-1];
}

void _fetchpin(){
    W=T;
    T=digitalRead(W);
}

void _pinMode(){
    W=T;
    DROP;
    pinMode(W, T);
    DROP;
}

void _storepin(){
    W=T;
    DROP;
    digitalWrite(W, T);
    DROP;
}

void _over(){
    W=stack[S-1];
    DUP;
    T=W;
}

void _sfetch(){
    W=S;
    DUP;
    T=W;
}

void _swap(){
    W=stack[S-1];
    stack[S-1]=T;
    T=W;
}

void _rot(){
    N=stack[S-2];
    W=stack[S-1];
    stack[S-1]=T;
    stack[S-2]=W;
    T=N;
}

void _twostar(){
    T=T<<1;
}

void _zeroequal(){
    if(T==0){
        T=0xffff;
        return;
    }
    T=0;
}

void _zeroless(){
    if(T<0){
        T=0xffff;
        return;
    }
    T=0;
}

void _minus(){
    W=T;
    DROP;
    T-=W;
}

void _equal(){
    T-=stack[--S];
    if(T==0){
        T=0xffff;
        return;
    }
    T=0;
}

void _litplus(){
    W=pgm_read_word(&memory[I++]);
    T+=W;
}

void _less(){
    W=T;
    DROP;
    if(T<W){
        T=0xffff;
        return;
    }
    T=0;
}

void _pstore(){
    P=T;
    DROP;
}

void _p(){
    DUP;
    T=P;
}

void _fetchpplus(){
    DUP;
    T=pgm_read_word(&memory[P++]);
}

void _fetchplus(){
    DUP;
    T=ram[A++];
    T|=ram[A++]<<8;
}

void _cfetchplus(){
    DUP;
    T=ram[A++];
}

void _astore(){
    A=T;
    DROP;
}

void _a(){
    DUP;
    T=A;
}

void _twoslash(){
    T=T/2;
}

void _cr(){
    Serial.write("\n");
}

void _dnumber(){
    DUP;
    T=Serial.parseInt(SKIP_WHITESPACE);
}

void _huh(){
    Serial.write(" ?\n");
    _abort();
}

void _plusbranch(){
    W=T;
    if(W&0x8000){
        I+=1;
        return;
    }
    I=pgm_read_word(&memory[I]);
}

void _nip(){
    S-=1;
}

void _invert(){
    T=T^0xffff;
}

void _variable(){
    DUP;
    T=pgm_read_word(&memory[W]);
}

void _Keyboardpress(){
//    Keyboard.press(T);
    DROP;
}

void _Keyboardrelease(){
//    Keyboard.release(T);
    DROP;
}

void _KeyboardreleaseAll(){
//    Keyboard.releaseAll();
}

void _storeaplus(){
    W=T;
    ram[A++]=W&0xff;
    ram[A++]=(T>>8)&0xff;
    DROP;
}

void _counter(){
    elapsed=millis();
}

void _timer(){
    Serial.print((millis()-elapsed)&0xffff);
}

void _false(){
    DUP;
    T=0;
}

void _true(){
    DUP;
    T=0xffff;
}

void _quit(){
    R=0;
    I=pgm_read_word(&memory[0]);
}

void _abort(){
    S=0;
    _quit();
}

void _nop(){
}

void (*function[])()={
    _enter , _exit , _emit , _ms , _branch , _0branch , _lit , _dot ,
    _dots , _dup , _drop , _plus , _and , _or , _xor , _key ,
    _execute , _fetchp , _cfetch, _fetch , _cstore , _store , _tor , _rfrom ,
    _rfetch , _next , _i2cread , _fetchpin , _pinMode , _storepin ,  _true , _false ,
    _over , _sfetch , _fetchpins , _swap , _rot , _twostar , _zeroequal , _zeroless ,
    _minus , _equal , _litplus , _less , _pstore , _p , _fetchpplus , _fetchplus ,
    _cfetchplus , _astore , _a , _twoslash , _cr , _dnumber , _huh , _abort ,
    _quit , _plusbranch , _nip , _invert , _nop , _dotsh , _initPins , _variable ,
    _Keyboardpress , _Keyboardrelease , _KeyboardreleaseAll , _counter , _timer ,
    _storeaplus ,
};

void _execute(){
    W=T;
    DROP;
    (*function[pgm_read_word(&memory[W++])])();
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
    (*function[pgm_read_word(&memory[W++])])();
    goto next;
/*    
ex:
//  if(W==0) goto abort;
  switch (pgm_read_word(&memory[W++])) {
    case 0: // enter
        _enter();
        goto next;
    case 1: // exit
        _exit();
        goto next;
    case 2: // emit
        _emit();
        goto next;
    case 3: // ms
        _ms();
        goto next;
    case 4: // branch
        _branch();
        goto next;
    case 5: // 0branch
        _0branch();
        goto next;
    case 6: // lit
        _lit();
        goto next;
    case 7: // .
        _dot();
        goto next;
    case 8: // .s
        _dots();
        goto next;
    case 9: // dup
        _dup();
        goto next;
    case 10: // drop
        _drop();
        goto next;
    case 11: // +
        _plus();
        goto next;
    case 12: // and
        _and();
        goto next;
    case 13: // or
        _or();
        goto next;
    case 14: // xor
        _xor();
        goto next;
    case 15: // key
        _key();
        goto next;
    case 16: // execute
        _execute();
//        W=T;
//        DROP;
//        goto ex;
        goto next;
    case 17: // @p
        _fetchp();
        goto next;
    case 18: // c@
        _cfetch();
        goto next;
    case 19: // @
        _fetch();
        goto next;
    case 20: // c!
        _cstore();
        goto next;
    case 21: // !
        _store();
        goto next;
    case 22: // >r
        _tor();
        goto next;
    case 23: // r>
        _rfrom();
        goto next;
    case 24: // r@
        _rfetch();
        goto next;
    case 25: // next
        _next();
        goto next;
    case 26: // @i2c port expander
        _i2cread();
        goto next;
    case 27: // @pin
        _fetchpin();
        goto next;
    case 28: // pinMode
        _pinMode();
        goto next;
    case 29: // !pin
        _storepin();
        goto next;
    case 30: // true
        _true();
        goto next;
    case 31: // false
        _false();
        goto next;
    case 32: // over
        _over();
        goto next;
    case 33: // S@
        _sfetch();
        goto next;
    case 34: // @pins 
        _fetchpins();
        goto next;
    case 35: // swap
        _swap();
        goto next;
    case 36: // rot
        _rot(); goto next;
    case 37: // 2*
        _twostar(); goto next;
    case 38: // 0=
        _zeroequal(); goto next;
    case 39: // 0<
        _zeroless(); goto next;
    case 40: // -
        _minus(); goto next;
    case 41: // =
        _equal(); goto next;
    case 42: // (#+)
        _litplus(); goto next;
    case 43: // <
        _less(); goto next;
    case 44: // p!
        _pstore(); goto next;
    case 45: // p
        _p(); goto next;
    case 46: // @p+
        _fetchpplus(); goto next;
    case 47: // @+
        _fetchplus(); goto next;
    case 48: // c@+
        _cfetchplus(); goto next;
    case 49: // a!
        _astore(); goto next;
    case 50: // a
        _a(); goto next;
    case 51: // 2/
        _twoslash(); goto next;
    case 52: // cr
        _cr(); goto next;
    case 53: // d#
        _dnumber(); goto next;
    case 54: // huh?
        _huh(); goto abort;
    case 55: // abort
        _abort();
        goto next;
    case 56: // quit
        _quit();
        goto next;
    case 57: // +branch
        _plusbranch(); goto next;
    case 58: // nip
        _nip(); goto next;
    case 59: // invert
        _invert(); goto next;
    case 60: //
        _nop();
        goto next;
    case 61: // .sh
        _dotsh(); goto next;
    case 62: // initPins
        _initPins(); goto next;
    case 63: // variable
        _variable(); goto next;
    case 64: // Keyboard.press
        _Keyboardpress(); goto next;
    case 65: // Keyboard.release
        _Keyboardrelease(); goto next;
    case 66: // Keyboard.releaseAll
        _KeyboardreleaseAll(); goto next;
    case 67: // !a+
        _storeaplus(); goto next;
    case 68: // counter
        _counter(); goto next;
    case 69: // timer
        _timer();
        goto next;
    case 70: // perform
//       _perform();
       _execute();
        goto next;
    default:
        goto abort;
  }  */
}
