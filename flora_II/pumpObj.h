#ifndef pumpObj_h
#define pumpObj_h

#include "blinker.h"

#define MOTOR_1_PIN 16
#define DEF_MOTORPULSE_PERCENT  10
#define DEF_MOTORPULSE_PERIOD   2000

class pumpObj : public blinker {

  public :
            pumpObj(int inPin,float inPercent,float inPeriodMs);
    virtual ~pumpObj(void);

    virtual void  setPercent(float percent);
    virtual void  setPeriod(float periodMs);
    virtual void  setPump(bool onOff);
    virtual bool  pumpOn(void);

            float mPercent;
};

extern bool    pumpCom; // Are we being told to turn the pump on by someone?
extern pumpObj ourPump;

#endif
