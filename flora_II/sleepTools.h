#ifndef sleepTools_h
#define sleepTools_h

#include "Arduino.h"
#include "timeObj.h"



class sleepMgr {

   public:
            enum sleepStates { awake, rampDown, sleeping };
            
            sleepMgr(void);
   virtual  ~sleepMgr(void);
   
            void  setupSleep(int awakeMS,int rampDownMs);
            void  shutOffSleep(void);
            float checkSleep(void);
            void  wakeup(void);
            
   protected:
            timeObj*    awakeTimer;
            timeObj*    rampDownTimer;
            sleepStates sleepState;
};

extern sleepMgr ourSleepMgr;



#endif
