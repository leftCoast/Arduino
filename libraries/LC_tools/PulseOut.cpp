#include "PulseOut.h"


PulseOut::PulseOut(byte inPin , float inPeriod) 
: 
timeObj(inPeriod),
idler() {

  pin = inPin;
  width = 0;
  init = false;
}


// Width is the width of the pulse.
void PulseOut::setWidth(float inWidth) {

  if (width!=inWidth) {
    if(width == 0) {                        // We are off, turn on!
      width = round(1000*inWidth);
      if (!init)  {                        // First time? Then initilize.
        hookup();                          // Set up idling.
        pinMode(pin, OUTPUT);              // Setup port.
        start();                          // Start the timer.
        init = true;                       // Note it.
      } 
    } 
    else
      width = round(1000*inWidth);
  } 
}


void PulseOut::idle(void) { 

  unsigned long startTime;
  unsigned long endPulse;

  Serial.println("servo idle");
  if(ding()) {
    startTime = micros();
    digitalWrite(pin, HIGH);
    endPulse = startTime + width;
    stepTime();
    while(micros() < endPulse);
    digitalWrite(pin, LOW);
  }
} 










