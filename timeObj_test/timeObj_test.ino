#include "timeObj.h"

timeObj theTimer(1000);

void setup() {

  pinMode(13, OUTPUT);
  theTimer.start();
}



void loop(){

  if (theTimer.ding()) {
    theTimer.stepTime();      // Step timer to next time..
    digitalWrite(13, HIGH);   // set the LED on
    delay(100);              // wait for a tenth second
    digitalWrite(13, LOW);    // set the LED off
  }

}

