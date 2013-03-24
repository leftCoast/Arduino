#include "blinker.h"

blinker aBlinker;                // Just use defaults. (Will blink pin 13 with built in LED)
blinker anotherBlinker(2);       // Choose a different pin?
blinker blinker3(3,1000,2000);    // Or, setup pin, on time & period..

void setup() {
  aBlinker.init();          // This gets it ready..
  aBlinker.setBlink(true);  // This starts the blinking..

  anotherBlinker.init();
  anotherBlinker.setBlink(true);

  blinker3.init();
  blinker3.setBlink(true);
}


void loop() {

  aBlinker.idle();          // Call this as much as possible to keep the blinker alive.
  anotherBlinker.idle();
  blinker3.idle();
}

