#include <string.h>
#include "parameters.h"
#include "pumpObj.h"


bool needReset;


paramType  params;

parameters::parameters(void) { needReset = false; }


parameters::~parameters(void) { }


// Store set of default parameters.
void parameters::floraReset(void) {
  
  params.moisture   = DEF_MOISTURE_LIMIT;     // Default numbers.
  params.waterTime  = DEF_WATER_TIME;
  params.soakTime   = DEF_SOAK_TIME;
  params.percent    = DEF_MOTOR_PWM_PERCENT;
  params.period     = DEF_MOTOR_PWM_PERIOD;
  strcpy(params.name,DEF_NAME);               // Our default name.
  saveParams();                               // Put these values out to "disk"                              
}


// Grab the set of params out of storage.
void parameters::readParams(void) {
    
    EEPROM.get(0,params);   // Read in this new set of params.
    needReset = true;       // Your params have changed, you need a reset.
}


// Stuff this set back into storage.
void parameters::saveParams(void) {
  
  EEPROM.put(0,params);
  needReset = true;     // Only time you save params is if something changed. So..
}


// Read the saved moisture value.
int parameters::getMoisture(void) { return params.moisture; }


// Read the saved water time value.
int parameters::getWaterTime(void) { return params.waterTime; }


// Read the saved soak time value.
int parameters::getSoakTime(void) { return params.soakTime; }


// Return a copy of the name string. YOU HAVE TO FREE IT!!
char* parameters::getName(void) { 
  char* newCopy;

  newCopy = (char*)malloc(strlen(params.name)+1);
  strcpy(newCopy,params.name);
  return newCopy;
}


// Read the saved PWM percent on value.
int parameters::getPWMPercent(void) { return params.percent; }


// Read the saved PWM percent peroid Ms value.
int parameters::getPWMPeriod(void) { return params.period; }


// Save a new moisture percent value.
void parameters::setMoisture(int percent) {

  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  params.moisture = percent;
  saveParams();         
}


// Save a new wating time, in Ms, value.
void parameters::setWaterTime(int waterMs) {
  
  if (waterMs<0) waterMs = 0;
  params.waterTime = waterMs;
  saveParams();
}


// Save a new soak time, in Ms, value.
void parameters::setSoakTime(int soakMs) {

  if (soakMs<0) soakMs = 0;
  params.soakTime = soakMs;
  saveParams();
}


// Save a new plant name.
void parameters::setName(char* nameStr) {

  if (strlen(nameStr)>=NAME_BUFF_BYTES) {
    nameStr[NAME_BUFF_BYTES] = '\0';
  }
  strcpy(params.name,nameStr);
  saveParams();
}


// Set a new PWM percent value. (pump speed)
void parameters::setPWMPercent(int percent) {
  
  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  params.percent = percent;
  saveParams();         
}


// Set a new PWM period value. (pump smoothness)
void parameters::setPWMPeriod(int periodMs) {

  if (periodMs<0) periodMs = 0;
  params.period = periodMs;
  saveParams();
}


parameters ourParamObj;
