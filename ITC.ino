// ITC.ino

const int PROGMEM memory[] = {
  5 , // reset  address 0
  3 , // lit    address 1
  2 , // branch address 2
  0 , // emit   address 3 
  1 , // ms     address 4
  1 , 65 ,   // lit 'A' to stack
  3 ,        // emit
  1 , 1000 , // lit 1000 to stack
  4 ,        // ms
  2 , 5 ,    // branch to address 5
};  // begin 65 emit 1000 ms again

