#include "timeObj.h"

// This blinks an LED without blocking. 
// onTimer is set to the time we want theLED ON.
// offTimer is set for the time we want to wait before turning it back on again. ( After its been turned off. )
// switchTimer is used to tell when to change test functions.
// test keeps track of what test function we're running at the moment.
// liteOn we use to keep track of whether the light is currently on or off.

timeObj onTimer(100);
timeObj offTimer(1000);
timeObj switchTimer(10000);

int     test;
bool    liteOn;

void setup() {

  pinMode(13, OUTPUT);      // Set up port 13, cause there's a light there.
  test = 0;
  digitalWrite(13, HIGH);   // set the LED ON
  liteOn = true;            // Note that the LED is ON.
}


void checkLight0(void) {
  
  if (liteOn && onTimer.ding()) {             // Its ON and the ON timer has expired.
    digitalWrite(13, LOW);                    // turn the LED OFF
    liteOn = false;                           // Note that the LED is OFF.
    offTimer.start();                         // Start off timer
  } else if (!liteOn && offTimer.ding()) {    // Its OFF and the OFF timer has expired.
    digitalWrite(13, HIGH);                   // set the LED ON
    liteOn = true;                            // Note that the LED is ON.
    onTimer.start();                          // Start ON timer.
  }
}


void checkLight1(void) {

  float currentTime;
  
  if (liteOn && onTimer.ding()) {             // Its ON and the ON timer has expired.
    digitalWrite(13, LOW);                    // turn the LED OFF
    offTimer.start();                         // Its off, start timing.
    liteOn = false;                           // Note that the LED is OFF.
    currentTime = onTimer.getTime()/1000.0;   // Grab the time, it'll be in micrseconds.
    onTimer.setTime(currentTime+20,false);     // Reset it as 10 ms more and auto start it.
   
  } else if (!liteOn && offTimer.ding()) {    // Its OFF and the OFF timer has expired.
    digitalWrite(13, HIGH);                   // set the LED ON.
    onTimer.start();                          // Start ON timer.
    liteOn = true;                            // Note that the LED is ON.
  }
}


void checkLights(void) {
  
  if (switchTimer.ding()) {             // We need to change tests now?
    switch(test) {                      // Guess so, what were we doing?
      case 0 : test = 1; break;         // In case 0 we just need to switch to test 1.
      case 1 : 
        onTimer.setTime(100);           // In case 1 we need to reset the onTimer before..
        test = 0;                       // Switching to case 0.
      break;
    }
    switchTimer.stepTime();             // And, sense it was time to switch, reset the switchTimer.
  }
  switch(test) {                        // Anyway, what are we doing now?
    case 0 : checkLight0(); break;      // In case 0 call checkLight0().
    case 1 : checkLight1(); break;      // In case 1 call checkLight1().
  }                                     // Easy peasy!
}


void loop(){

  checkLights();  // This deals with the blinking the light with no blocking.
  
  // Do other things..
}
