#ifndef pumpObj_h
#define pumpObj_h

#include "blinker.h"

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

#endif
