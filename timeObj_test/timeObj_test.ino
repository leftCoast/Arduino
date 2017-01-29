#include "timeObj.h"

// This blinks an LED without blocking. 
// onTimer is set to the time we want theLED ON.
// offTimer is set for the time we want to wait before turning it back on again. ( After its been turned off. )
// liteOn we use to keep track of whether the light is currently on or off.

timeObj onTimer(100);
timeObj offTimer(1000);
bool    liteOn;

void setup() {

  pinMode(13, OUTPUT);      // Set up port 13, cause there's a light there.
  
  digitalWrite(13, HIGH);   // set the LED ON
  onTimer.start();          // Start ON timer
  liteOn = true;            // Note that the LED is ON.
}



void loop(){

  if (liteOn && onTimer.ding()) {             // Its ON and the ON timer has expired.
    digitalWrite(13, LOW);                    // turn the LED OFF
    offTimer.start();                         // Start off timer
    liteOn = false;                           // Note that the LED is OFF.
  } else if (!liteOn && offTimer.ding()) {    // Its OFF and the OFF timer has expired.
    digitalWrite(13, HIGH);                   // set the LED ON
    onTimer.start();                          // Start ON timer
    liteOn = true;                            // Note that the LED is ON.
  }
  // Do other things..
}

