#include <slowServo.h>


// Constructor, Lets have a pin number to play with.
slowServo::slowServo(int inPin) {
  
  servoPtr    = NULL;   // We have no servi object yet.
  pin         = inPin;  // Here's our pin number, save that.
  MsDelay     = 1;      // set dlay to a default of 1 Ms.
  currentDeg  = 0;      // set current degree to zero.
  desiredDeg  = 0;      // set default desired degree to zero.
}

// Destructor, recycle the servo onbject.
slowServo::~slowServo(void) { if (servoPtr) delete(servoPtr); }

// Yes this servo thing needs a begin call.
void slowServo::begin(void) {

  if (!servoPtr) {                  // If seroPtr is NULL..
    servoPtr = new Servo;           // Create a new servo object.
    servoPtr->attach(pin);          // Do the attach pin thing.                       
    servoPtr->write(currentDeg);    // Set our default locaton.
    degTimer.setTime(MsDelay);      // Set our defualt delay time.
    hookup();                       // Hook us into the idle queue. (ignition ON!)
  }                                 // If we already have a servo object? Don't bother.
}


// Set a new speed. IE Ms delay per degree.
void slowServo::setMsPerDeg(int inMs) { MsDelay = inMs; }


// Set a new position. IE Degre rotation.
void slowServo::setDeg(int inDeg) { desiredDeg = inDeg; }


// Run the machine. This gets called automatically by the
// idle(); command in your sketch's loop. It will NOT be
// called if begin is not called to set things up beforehand.
void slowServo::idle(void) {

  if (currentDeg!=desiredDeg && degTimer.ding()) {
    if (currentDeg>desiredDeg) {
      currentDeg--;
    } else {
      currentDeg++;
    }
    servoPtr->write(currentDeg);
    degTimer.setTime(MsDelay,true);
  }
}