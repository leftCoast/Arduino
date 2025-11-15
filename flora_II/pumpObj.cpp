#include "pumpObj.h"
#include <timeObj.h>


pumpObj ourPump(MOTOR_1_PIN,DEF_MOTOR_PWM_PERCENT,DEF_MOTOR_PWM_PERIOD);


pumpObj::pumpObj(int inPin,float inPWMPercent,float inPWMPeriodMs)
  : squareWave(inPWMPeriodMs,inPWMPeriodMs*(inPWMPercent/100)) {

  mInit = false;
  mPin = inPin;
  //setPulse(DEF_MOTOR_PWM_PERCENT,DEF_MOTOR_PWM_PERCENT,2000); // Default it just run wide open.
  setPulse(100,35,1500);
  mSpeedTimer.setTime(DELTA_SPEED_TIME,false);
}

    
pumpObj::~pumpObj(void) { }


void pumpObj::setPump(bool onOff) {

  if (onOff) {                // If theyt want the pump on..
   if (!mInit) {              // If not initialzed..
      pinMode(mPin, OUTPUT);  // Now you are!
      mInit = true;           // Flag it!
   }
    mSpeedTimer.start();      // Start up the timer!
    mSpeedFrame = 0;          // Reset the speed profile.
    setSpeed();               // Set the speed.
  }
  setOnOff(onOff);            // Turn the pump on or off.
 }


bool pumpObj::pumpOn(void) { return running(); }

                              //100                  35              1500
void pumpObj::setPulse(float inHighPercent,float inLowPercent,float inPeriodMs) {

  mPeriod = inPeriodMs;
  mSpeedPlot.clearMap();
  mSpeedPlot.addPoint(0,inHighPercent);
  mSpeedPlot.addPoint(inPeriodMs/2.0,inHighPercent);
  mSpeedPlot.addPoint((inPeriodMs/2.0)+1,inLowPercent);
  mSpeedPlot.addPoint(inPeriodMs,inLowPercent);
  mSpeedFrame = 0;
}


void pumpObj::setSpeed(void) {
  
  if (mSpeedTimer.ding()||mSpeedFrame==0) {
    if (mSpeedFrame>mPeriod) {
      mSpeedFrame = 0;
    }
    setPercent(mSpeedPlot.map(mSpeedFrame));
    mSpeedFrame = mSpeedFrame + DELTA_SPEED_TIME;
    if (mSpeedTimer.ding()) mSpeedTimer.stepTime();
  }
}


void pumpObj::pulseOn(void) { digitalWrite(mPin,HIGH); }


void pumpObj::pulseOff(void) { digitalWrite(mPin,LOW); }
