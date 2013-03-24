
#include <arduino.h>
#include "blinker.h"

blinker::blinker(int inPin,float inOnMs, float inPeriodMs) : 
timeObj(inPeriodMs) {


  init = false;
  running = false;
  pinHigh = false;
  pin = inPin;
  onTimer = new timeObj(inOnMs);
}


void blinker::setBlink(boolean onOff) {

  
  if(onOff != running) {		// ignore if no change
    if (onOff) {			// Start blinking..
      if (!init) {                      // Not intialized?
        hookup();                       // Set up idling.
        pinMode(pin, OUTPUT);           // Now you are!
        init = true;                    // Note it.
      }
      start();                          // Starting NOW!
      digitalWrite(pin,HIGH);	        // light on!
      onTimer->start();                 // set the time on timer.
      pinHigh = true;		        // set state.
      running = true;
    } 
    else {			         // Stop blinking..
      digitalWrite(pin,LOW);		// light off.
      running = false;		        // set state.
      pinHigh = false;
    }
  }
}


void blinker::idle(void) {

  if (running) {
    if(pinHigh) {                     // light is on.
      if (onTimer->ding()) {          // time to turn it off..
        digitalWrite(pin,LOW);	      // light off!
        pinHigh = false;              // note that.
      }
    } 
    else {                           // light's off
      if (ding()) {                  // time to turn it on!
        digitalWrite(pin,HIGH);      // on it goes.
        onTimer->start();            // set the time on timer.
        pinHigh = true;		     // set state.
        stepTime();                  // Reset timer for next on cycle.
      }
    }
  }
}


void blinker::setTimes(float inOnMs, float inPeriodMs) {

  onTimer->setTime(inOnMs);
  timeObj::setTime(inPeriodMs);
}



