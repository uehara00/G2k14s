// G2k14s
// simple demonstrations for 14-segment LED modules provided by Ghz2000
//
#include <Arduino.h>
#include <G2k14s.h>

void setup() {
}

void loop() {
  G2k14s.lamp_test(  300 );
  G2k14s.char_test( 1000 );
  G2k14s.flow( 250, "14-Segment LED modules from Ghz2000 controlled by Arduino." );
  G2k14s.flow( 250, "An old saying tells us that, if you want something done well, you should do it yourself." );
}

