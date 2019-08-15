#ifndef pumpObj_h
#define pumpObj_h

#include "blinker.h"
#include "multiMap.h"

// First block here is saved in the parameters.
#define DEF_MOTOR_PWM_PERCENT   100     // Basicaly throttle setting.
#define DEF_MOTOR_PWM_PERIOD    20      // PWM peroid. Smooth with control is the goal.


#define MOTOR_1_PIN             16      // Pin the motor runs on. (Hard coded)
#define DELTA_SPEED_TIME        250     // How often do we plan on changing throttle settings, ms. (Also hard coded for now)


class pumpObj : public blinker {

  public :
            pumpObj(int inPin,float inPWMPercent,float inPWMPeriodMs);
    virtual ~pumpObj(void);

    virtual void  setPWMPercent(float percent);                                       // First two are for setting up the PWM for controlling motor power/speed.
    virtual void  setPWMPeriod(float periodMs);
    virtual void  setPump(bool onOff);                                                // Turn the pump on or off.
    virtual bool  pumpOn(void);                                                       // Is the pump on?
    virtual void  setPulse(float inHighPercent,float inLowPercent,float inPeriodMs);  // This is for setting up speed variance over time.
    virtual void  setSpeed(void);                                                     // Dynamic speed gets set in here.
    
            float     mPWMPercent;                                                    // The PWM Stuff is all about high freq. motor speed control.
            float     mHigh;                                                          // These are low freq. numbers for speed variance over time.
            float     mLow;
            float     mPeroid;
            multiMap  mSpeedPlot;
            timeObj   mSpeedTimer;
            int       mSpeedFrame;
};

extern bool    pumpCom; // Are we being told to turn the pump on by someone?
extern pumpObj ourPump;

#endif
