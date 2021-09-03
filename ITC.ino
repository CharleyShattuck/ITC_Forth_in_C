// ITC.ino

const int PROGMEM memory[] = {
  7 , // reset  address 0
  3 , // lit    address 1
  2 , // branch address 2
  0 , // emit   address 3 
  1 , // ms     address 4
  4 , // dup    address 5
  5 , // 1+     address 6
  1 , 65 ,   // lit 'A' to stack
  6 ,        // 1+
  5 ,        // dup
  3 ,        // emit
  1 , 1000 , // lit 1000 to stack
  4 ,        // ms
  2 , 8 ,    // branch to address 8
};  // begin 65 emit 1000 ms again

