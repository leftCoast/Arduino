#include "pumpObj.h"
#include <timeObj.h>

bool    pumpCom;
pumpObj ourPump(MOTOR_1_PIN,DEF_MOTORPULSE_PERCENT,DEF_MOTORPULSE_PERIOD);


pumpObj::pumpObj(int inPin,float inPercent,float inPeriodMs)
  : blinker(inPin) {
    
  if (inPercent<0) inPercent = 0;
  if (inPercent>100) inPercent = 100;
  mPercent = inPercent;
  setPeriod(inPeriodMs);
  setPercent(inPercent);
  pumpCom = false;
}

    
pumpObj::~pumpObj(void) { }


void pumpObj::setPercent(float percent) {

  float periodMs;
  float onMs;
  
  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  periodMs = getTime()/1000;
  onMs = (percent/100)*periodMs;
  setTimes(onMs,periodMs);
  mPercent = percent;
}


void pumpObj::setPeriod(float periodMs) {

  float onMs;
  
  onMs = (mPercent/100)*periodMs;
  setTimes(onMs,periodMs);
}


void pumpObj::setPump(bool onOff) { setBlink(onOff); }


bool pumpObj::pumpOn(void) { return running; }
