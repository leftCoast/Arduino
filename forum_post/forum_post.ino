#include <blinker.h>
#include <colorObj.h>
#include <debug.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <squareWave.h>
#include <textBuff.h>
#include <timeObj.h>


class timeoutBlinker :  public blinker,
                        public timeObj {
   public:
                  timeoutBlinker(int pin,float timeOutMs,float blinkOnMs,float periodMs);
         virtual  ~timeoutBlinker(void);

         virtual  void  setOnOff(bool onOff);  
         virtual  void  idle(void);        
};


timeoutBlinker::timeoutBlinker(int pin,float timeOutMs,float blinkOnMs,float periodMs)
   : blinker(pin,blinkOnMs,periodMs),
   timeObj(timeOutMs) {  }

   
timeoutBlinker::~timeoutBlinker(void) {  }

void timeoutBlinker::setOnOff(bool onOff) {

   if (onOff) start();
   blinker::setOnOff(onOff);
}

void  timeoutBlinker::idle(void) {

   if (blinking()) {
      if (ding()) {
         setOnOff(false);
      }
   }
   blinker::idle();    
}


timeoutBlinker ourBlinker(13,5000,20,100);

void setup(void) {

   pinMode(15, INPUT_PULLUP);
}


void loop(void) {

   idle();
   if (!digitalRead(15)) {
      while(!digitalRead(15));
      if (ourBlinker.blinking()) {
         ourBlinker.setOnOff(false);
         else
         ourBlinker.setOnOff(true);
   }
}
