#include "timeObj.h"

timeObj onTimer(100);
timeObj offTimer(1000);
bool    liteOn;

void setup() {

  pinMode(13, OUTPUT);      // Set up port 13
  
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

