#include "timeObj.h"
#include <arduino.h>

timeObj::timeObj(float inMs) {

  startTime = 0;
  endTime = 0;
  crossing = false;
  setTime(inMs);        // This sets startTime and endTime. Best go here?
}


void timeObj::setTime(float inMs,bool startNow) {

  if (inMs>0)
    waitTime = round(1000*inMs);
  else
    waitTime = 0;
  if (startNow) start();
}


void timeObj::start(void) {

  startTime = micros();
  endTime = startTime + waitTime;
  crossing = endTime < startTime;
}


void timeObj::stepTime(void) {

  startTime = startTime + waitTime;
  endTime = startTime + waitTime;
  crossing = endTime < startTime;
}


bool timeObj::ding(void) {
  
  unsigned long now;

  now = micros();
  if (crossing)
    return (now < startTime) && (now >= endTime);
  else
    return now >= endTime;
}


unsigned long timeObj::getTime(void) { return waitTime; }

