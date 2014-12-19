
#include <arduino.h>
#include "blinker.h"

blinker::blinker(int inPin,float inOnMs, float inPeriodMs,boolean inInverse) :
timeObj(inPeriodMs) {


  init = false;
  running = false;
  lightOn = false;
  pin = inPin;
  setInverse(inInverse);
  onTimer = new timeObj(inOnMs);
}


void blinker::setInverse(boolean inInverse) { inverse = inInverse; }


void blinker::setLight(boolean onOff) {
    
    if(onOff) { // light on!
        if (inverse) digitalWrite(pin,LOW);
        else digitalWrite(pin,HIGH);
    } else { // light off!
        if (inverse) digitalWrite(pin,HIGH);
        else digitalWrite(pin,LOW);
    }
    lightOn = onOff;
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
      setLight(true);                   // light on!
      onTimer->start();                 // set the time on timer.
      running = true;
    } 
    else {			         // Stop blinking..
      setLight(false);                  // light off.
      running = false;		        // set state.
    }
  }
}


void blinker::idle(void) {

  if (running) {
    if(lightOn) {                     // light is on.
      if (onTimer->ding()) {          // time to turn it off..
        setLight(false);              // light off.
      }
    } 
    else {                           // light's off
      if (ding()) {                  // time to turn it on!
        setLight(true);              // on it goes.
        onTimer->start();            // set the time on timer.
        stepTime();                  // Reset timer for next on cycle.
      }
    }
  }
}


void blinker::setTimes(float inOnMs, float inPeriodMs) {

  onTimer->setTime(inOnMs);
  timeObj::setTime(inPeriodMs);
}



