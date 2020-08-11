#include <blinker.h>

blinker aBlinker;                         // Just use defaults. (Will blink pin 13 with built in LED)
blinker anotherBlinker(2);                // Choose a different pin?
blinker blinker3(3,1000,2000);            // Or, setup pin, on time & period..

void setup() {

  aBlinker.setOnOff(true);  // This starts the blinking..
  anotherBlinker.setOnOff(true);
  blinker3.setOnOff(true);
}


void loop() {

  idle();          // Call this as much as possible to keep the blinker alive.
}
