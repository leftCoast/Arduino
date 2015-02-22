#include "sonarObj.h"
#include "mapper.h"

#define pulseOut   5      //µs
#define delayTime 0.2     //ms
#define timeoutTime  5    //ms
#define pulseMin   115    //µs
#define pulseMax   18500  //µs
#define minCm      2      //cm
#define maxCm      300    //cm


// *****************************
// Glue code for intrupt handling

volatile unsigned long time0H;
volatile unsigned long time0L;
volatile unsigned long time1H;
volatile unsigned long time1L;


void handler0() { 
  if (!time0H) {
    time0H = micros();
  } 
  else if(!time0L) {
    time0L = micros();
  }
}


void handler1() { 
  if (!time1H) {
    time1H = micros();
  } 
  else if(!time1L) {
    time1L = micros();
  }
}


//*****************************
// The sonarObj stuff..

mapper cmMapper(pulseMin,pulseMax,minCm,maxCm);

enum { 
  ready, waitforPulse, waitToComplete };

sonarObj::sonarObj(int inPin) {
  
  timeoutTimer = new timeObj(timeoutTime);
  endTimer = new timeObj(delayTime);
  pin = inPin;
  result = 0;  // default value.
  state = ready;
  isAttached = false;
}


sonarObj::~sonarObj(void) {
  
  delete(timeoutTimer);
  delete(endTimer);
  if (isAttached) {
    detachHandler();
  }
}


// Start timing the incoming pulse..
void sonarObj::attachHandler(void) {

  switch (pin) {
  case 2 :
    EIFR = _BV (INTF0);  // clear flag for interrupt 0
    attachInterrupt(0, handler0, CHANGE);
    isAttached = true;
    break;
  case 3 :
    EIFR = _BV (INTF1);  // clear flag for interrupt 1
    attachInterrupt(1, handler1, CHANGE);
    isAttached = true;
    break;
  }
}


// We are done with the handler now. Let it go.
void sonarObj::detachHandler(void) {

  switch (pin) {
  case 2 :
    detachInterrupt(0);
    isAttached = false;
    break;
  case 3 :
    detachInterrupt(1);
    isAttached = false;
    break;
  }
}


// If we have non zero values the pulse has passed.
bool  sonarObj::pulseDone(void) {

  switch (pin) {
  case 2 :
    return time0L && time0H;  // When both are non zero we are done.
  case 3 :
    return time1L && time1H;
  }
}  


// How long wa the pulse?
long sonarObj::readTime(void) {

  switch (pin) {
  case 2 :
    return time0L - time0H;
  case 3 :
    return time1L - time1H;
  }
}


// Set them both to zero.
void sonarObj::clearTime(void) {

  switch (pin) {
  case 2 :
    time0L = 0;
    time0H = 0;
    break;
  case 3 :
    time1L = 0;
    time1H = 0;
    break;
  }
}


void sonarObj::idle(void) {

  if (pin==2||pin==3) {
    switch (state) {
    case ready :
      clearTime();                    // Set both to zero as flags..
      timeoutTimer->start();          // I suspect its missing the pulse & timing out. We'll see..
      state = waitforPulse;           // All done, let the processor do other things.
      pinMode(pin, OUTPUT);           // Pulse out to start hardware.
      digitalWrite(pin, HIGH);
      delayMicroseconds(pulseOut);
      digitalWrite(pin, LOW);
      pinMode(pin, INPUT);            // I assume this is ok, we are going to listen.
      attachHandler();                // Hook up the handler.
      break;
    case waitforPulse :
      if (pulseDone()) {                     // Checks to see if both are non zero.
        detachHandler();                     // Maybe someone else needs this.
        endTimer->start();                   // we need to wait for a minimum settle time.
        if (readTime() > 0)  {               // If the numbers make sense, do the calculations.
          result = cmMapper.Map(readTime());
        }
        state = waitToComplete;              // Wait for the settling timer expires.
      } else if (timeoutTimer->ding()) {
        detachHandler();                     // Maybe someone else needs this.
        endTimer->start();                   // we need to wait for a minimum settle time.
        state = waitToComplete;              // Wait for the settling timer expires.
      }
      break;
    case waitToComplete :
      if (endTimer->ding())                  // If the timer dings, we are done. Reset state and go.
        state = ready;
      break;
    }
  }
}


float sonarObj::distanceCm(void) {

  return result;
}











