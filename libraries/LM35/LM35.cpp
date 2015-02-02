#include "LM35.h"
#include <mapper.h>

// We all need the mappers and tey are always the same. So just
// make up the two and share em.
mapper intToVolts(0,1023,0,5.0);    // From Arduinoland..
mapper voltsToTemp(0,1.5,0,150);    // From the LM35 datasheet.


// Constructor, saves off our input pin.
LM35::LM35(int inPin) { pin = inPin; }

// Destructor, nothing to see here..
LM35::~LM35(void) { }
 

// How to convert centagratde to Farenheight. From the web.
float LM35::centToDegF(float cent) { return (cent * 1.8) + 32; }
 

// This is the only call that actually talks to the sensor.
// Returns the temp. in centagrade.
float LM35::tempC(void) {
  
  unsigned int count = analogRead(pin);
  double volts = intToVolts.Map(count);
  return voltsToTemp.Map(volts);
}
  
// To get Farenheight we first grab centagrade then convert
// the value. If you want, you can do this yourself using
// the centToDegF(tempC); function.
float LM35::tempF(void) { return centToDegF(tempC()); }

