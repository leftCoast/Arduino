#ifndef miniSSC_h
#define miniSSC_h

#include <Arduino.h>
#include "mapper.h"

#define miniSSCMinVal -1
#define miniSSCMaxVal 1
#define NUM_SERVOS 18 //255

class miniSSC  {

public:
  miniSSC(void);

  void initSSC(bool fast=false);
  void setServo(byte servoNum,float inVal);  // Value ranged from -1.0 ... 1.0
private :
  
  byte buff[3];
  byte lastVal[NUM_SERVOS];
  
};

#endif

