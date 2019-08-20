#include "pumpObj.h"
#include <timeObj.h>

bool    pumpCom;
pumpObj ourPump(MOTOR_1_PIN,DEF_MOTOR_PWM_PERCENT,DEF_MOTOR_PWM_PERIOD);


pumpObj::pumpObj(int inPin,float inPWMPercent,float inPWMPeriodMs)
  : blinker(inPin) {
    
  if (inPWMPercent<0) inPWMPercent = 0;
  if (inPWMPercent>100) inPWMPercent = 100;
  mPWMPercent = inPWMPercent;
  setPWMPeriod(inPWMPeriodMs);
  setPWMPercent(inPWMPercent);
  //setPulse(DEF_MOTOR_PWM_PERCENT,DEF_MOTOR_PWM_PERCENT,2000); // Default it just run wide open.
  setPulse(100,35,1500);
  mSpeedTimer.setTime(DELTA_SPEED_TIME,false);
  pumpCom = false;
}

    
pumpObj::~pumpObj(void) { }


void pumpObj::setPWMPercent(float percent) {

  float periodMs;
  float onMs;

  if (percent<0) percent = 0;
  if (percent>100) percent = 100;
  periodMs = getTime()/1000;
  onMs = (percent/100)*periodMs;
  setTimes(onMs,periodMs);
  mPWMPercent = percent;
}


void pumpObj::setPWMPeriod(float periodMs) {

  float onMs;
  
  onMs = (mPWMPercent/100)*periodMs;
  setTimes(onMs,periodMs);
}


void pumpObj::setPump(bool onOff) {

  if (onOff) {
    mSpeedTimer.start();
    mSpeedFrame = 0;
    setSpeed();
  }
  setBlink(onOff);
 }


bool pumpObj::pumpOn(void) { return running; }


void pumpObj::setPulse(float inHighPercent,float inLowPercent,float inPeriodMs) {

  mHigh   = inHighPercent;
  mLow    = inLowPercent;
  mPeroid = inPeriodMs;
  mSpeedPlot.clearMap();
  mSpeedPlot.addPoint(0,mHigh);
  mSpeedPlot.addPoint(mPeroid/2.0,mHigh);
  mSpeedPlot.addPoint((mPeroid/2.0)+1,mLow);
  mSpeedPlot.addPoint(mPeroid,mLow);
  mSpeedFrame = 0;
}


void pumpObj::setSpeed(void) {
  
  if (mSpeedTimer.ding()||mSpeedFrame==0) {
    if (mSpeedFrame>mPeroid) {
      mSpeedFrame = 0;
    }
    setPWMPercent(mSpeedPlot.Map(mSpeedFrame));
    mSpeedFrame = mSpeedFrame + DELTA_SPEED_TIME;
    if (mSpeedTimer.ding()) mSpeedTimer.stepTime();
  }
}
