#include "LM35.h"


LM35::LM35(int inPin) {
  
    pin = inPin;
    intToVolts = new mapper(0,1023,0,5.0);
    voltsToTemp = new mapper(0,1.5,0,150);
}


LM35::~LM35(void) {
   
   delete(intToVolts);
   delete(voltsToTemp);
}
 
 
float LM35::centToDegF(float cent) { return (cent * 1.8) + 32; }
 
 
float LM35::tempC(void) {
  
  unsigned int count = analogRead(pin);
  double volts = intToVolts->Map(count);
  return voltsToTemp->Map(volts);
}
  
  
float LM35::tempF(void) { return centToDegF(tempC()); }

