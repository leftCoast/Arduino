#include "squareWave.h"
#include "blinker.h"


// ************************************************************** 
// doubleWave class
// **************************************************************


class doubleWave : public blinker {

   public:
               doubleWave(int pinNum, float hfPulseMs, float hfPeriodMs,float lfPulseMs,float lfPeriodMs);
   virtual     ~doubleWave(void);

               void  trigger(bool onOff);
   virtual     void  pulseOn(void);
   virtual     void  pulseOff(void);
   
               blinker     hfWave;
};

               
doubleWave::doubleWave(int pinNum, float hfPulseMs,float hfPeriodMs,float lfPulseMs, float lfPeriodMs)
   : blinker(pinNum,lfPulseMs,lfPeriodMs) {
   
   hfWave.setPeriod(hfPeriodMs);
   hfWave.setPulse(hfPulseMs);
}

   
doubleWave::~doubleWave(void) {  }


void doubleWave::pulseOn(void) { hfWave.setOnOff(true); }
void doubleWave::pulseOff(void) { hfWave.setOnOff(false); }          


   
// **************************************************************
// And we make it go..
// **************************************************************



doubleWave blinky(13,20,100,1000,2000); // Devine your double waves. (YOu can crreate as many as you like..)


void setup() {
   
   blinky.setOnOff(true);  // Turn it on..
}

void loop() {
  
  idle();                  // Let the idlers have some time.
}
