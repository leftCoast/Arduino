#include <string.h>
#include "parameters.h"
#include "pumpObj.h"


bool needReset;

paramType  params;

parameters::parameters(void) { needReset = false; }


parameters::~parameters(void) { }


// Store set of default parameters.
void parameters::floraReset(void) {
  
  params.dryLimit   = DEF_DRY_LIMIT;          // Default numbers.
  params.waterTime  = DEF_WATER_TIME;
  params.soakTime   = DEF_SOAK_TIME;
  params.percent    = DEF_MOTOR_PWM_PERCENT;
  params.period     = DEF_MOTOR_PWM_PERIOD;
  params.dry        = DEF_DRY;
  params.mud        = DEF_MUD;
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
int parameters::getDryLimit(void) { return params.dryLimit; }


// Read the saved water time value.
int parameters::getWaterTime(void) { return params.waterTime; }


// Read the saved soak time value.
int parameters::getSoakTime(void) { return params.soakTime; }


// Return a pointer to the name string. DON'T FREE IT!!
char* parameters::getName(void) { return params.name; }


// Read the saved PWM percent on value.
int parameters::getPWMPercent(void) { return params.percent; }


// Read the saved PWM percent peroid Ms value.
int parameters::getPWMPeriod(void) { return params.period; }


// Read the saved "dry" capacitance level.
int parameters::getDry(void) { return params.dry; }


// Read the saved "mud" capacitance level.
int parameters::getMud(void) { return params.mud; }


// Save a new moisture percent value.
void parameters::setDryLimit(int percent) {

  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  params.dryLimit = percent;
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


// Set a new "dry" cap value.
void parameters::setDry(int dryCap) {

  if (dryCap<0) dryCap = 0;
  params.dry = dryCap;
  saveParams();
}


// Set a new "dry" cap value.
void parameters::setMud(int mudCap) {

  if (mudCap<0) mudCap = 0;
  params.mud = mudCap;
  saveParams();
}


parameters ourParamObj;
