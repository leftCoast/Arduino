#ifndef PulseOut_h
#define PulseOut_h

#include <Arduino.h>
#include "timeObj.h"
#include "idlers.h"

// ONLY USE THIS FOR TIGHT PULSE TIMING
// IT BLOCKS WHEN THE PULSE IS HIGH.

class PulseOut : public timeObj, public idler {
   
  public:
              PulseOut(byte inPin , float inPeriod,boolean inverse=false);   // inPeriod in ms
              void setWidth(float inWidth);            // inWidth in ms
      virtual void idle(void);
   
   private:
      boolean       init;
      byte          pin;
      unsigned long width;
	  boolean		pullDown;
};
   
#endif
