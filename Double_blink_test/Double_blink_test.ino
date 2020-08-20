#include "blinker.h"
#include "idlers.h"


class delayWave : public blinker {
   
   public:
            enum waveState { startingUp, changingDelay, sitting, running };
            
            delayWave(int pin1,int pin2,float pulseMs,float periodMs,float delayMs);
            ~delayWave(void);

            void     setDelay(float delayMs);
            void     startStop(bool onOff);

   protected:
            void     startCycle(void);
   virtual  void     pulseOn(void);
   virtual  void     idle();

            waveState   mWaveState;
            blinker*    mSecondLED;
            float       mMinDelay;
            float       mMaxDelay;
            timeObj     mDelay;
            float       mDelayMs;
};



delayWave::delayWave(int pin1,int pin2,float pulseMs,float periodMs,float delayMs)
   : blinker(pin1,pulseMs,periodMs) {

   mSecondLED  = new blinker(pin2,pulseMs,periodMs);  // Second blinker.
   mMinDelay   = pulseMs;                             // Calculate our minimum delay.
   mMaxDelay   = periodMs - 2*pulseMs;                // Calculate our maximum delay.
   mDelayMs    = 0;                                   // 
   mWaveState  = sitting;                             // Well, we're are.
   setDelay(delayMs);                                 // Calculate our initial delay setting.
}


// If we have the second blinker, recycle it.
delayWave::~delayWave(void) { if (mSecondLED) delete(mSecondLED); }


// User wants to delay the second wave differently.. We'll take note of it for later.
void delayWave::setDelay(float delayMs) {

   if (delayMs<mMinDelay) {               // Sanity first. If delayMs is less than the minimum linit..
      delayMs = mMinDelay;                // Set it to the minimum limit.
   } else if (delayMs>mMaxDelay) {        // If delayMs is greater than the max linit..
      delayMs = mMaxDelay;                // Set it to the max limit.
   }
   if (delayMs!=mDelayMs) {               // Now, if its different than what we are running now..
      mDelayMs = delayMs;                 // Set in the new number.
      switch (mWaveState) {               // Check the state we are in.
         case startingUp      :           // We're already starting up..
         case running         :           // Or we're running..
            mWaveState = changingDelay;   // Note change to be caught when our pulse is complete.
         break;                           // And we are done.
         case changingDelay   :           // Already noted I see..
         case sitting         : break;    // Or we are just sitting? Don't need to do anything more.
      }
   }
}


// User switch for turning this thing on or off.
void delayWave::startStop(bool onOff) {
   
   if (onOff) {                           // They want to turn us on. Eww!
      hookup();                           // When it comes on, we need to hook into the idle queue.
      switch (mWaveState) {               // Check the state we are in.
         case startingUp      :           // We're already starting up, be patient.
         case changingDelay   :           // Again, what with this? We're already running.
         case running         : break;    // Duh! running. (all these cases we do nohing.)
         case sitting         :           // Ok, in this case, call the startCycle() method.
            startCycle();                 // This fires up the start sequence,
         break;                           // And we're done.
      }
   } else {                               // Else, they want us gone, Aww..
      setOnOff(false);                    // Shut down ourselves.
      mSecondLED->setOnOff(false);        // Shut down the second blinker.
      mWaveState = sitting;               // Note we're off.
   }
}



// Trapping when the pulse completes so we can change the delay. (if needed)
void  delayWave::pulseOn(void) {

   blinker::pulseOn();                          // Our pulse is over, let our ancestor do their stuff.
   if (mWaveState==changingDelay) {             // if we need to change the delay of the second signal..
      mDelay.setTime(mDelayMs+mNextPulse,true); // Fire up our delay timer. (Using mNextPulse is a hack, sorry.)
      mWaveState = startingUp;                  // Note what we're up to for the guys downstairs. 
   }
}


// We don't really care what's going on, just start things up.
// We first shut things down if they are on. Then do the start up routine.
void delayWave::startCycle(void) {
   Serial.println("Our State.");
   printState();
   Serial.println("Second state.");
   mSecondLED->printState();
   
   setOnOff(false);                          // Shut off the first blinker. (ourselves)
   mSecondLED->setOnOff(false);              // Shut down the second blinker.
   setOnOff(true);                           // Fire up the first blinker. (ourselves)
   mDelay.setTime(mDelayMs,true);            // Fire up our delay timer. Just the length of the delay.
   mWaveState = startingUp;                  // Note what we're up to for the guys downstairs. 
   
   Serial.println("-----");
   Serial.println("Our State.");
   printState();
   Serial.println("Second state.");
   mSecondLED->printState();
}


void delayWave::idle() {

   blinker::idle();                       // Ancester goes first, we did bypass the poor thing.
   switch(mWaveState) {                   // Check our state..
      case sitting         :              // Sitting? Do nothing.
      case running         : break;       // Running? The acestor dealt with that.
      case startingUp      :              // Starting up? We need to deal with this.
         if (mDelay.ding()) {             // If the dealy timer dinged..
            mSecondLED->setOnOff(true);   // Fire up the second blinker.
            mWaveState = running;         // Set our state to running.
         }
         break;                           // A good days work completed..
      case changingDelay   : break;       // We ignore this too, coming off the pulse will take action in this case.
   }
}



delayWave* ourWaveObj;
            
void setup() {

   Serial.println("---\\\ In setup, calling delayWave();\\\---");Serial.flush(); ourWaveObj = new delayWave(13,16,2000,8000,3000);
   Serial.println("---\\\ In setup, calling startStop();\\\---");Serial.flush(); ourWaveObj->startStop(true);
}


void loop() {
   //idle();
   // Do whatever else you want. Just NOT delay();
}
