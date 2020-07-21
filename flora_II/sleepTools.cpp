#include "sleepTools.h"


sleepMgr::sleepMgr(void) {
   
   awakeTimer      = NULL;
   rampDownTimer  = NULL;
}


sleepMgr::~sleepMgr(void) { 

  if (awakeTimer) {
      delete(awakeTimer);
      awakeTimer = NULL;
   }
   if (rampDownTimer) {
      delete(rampDownTimer);
      rampDownTimer = NULL;
   }
}
  
// If we want to run a sleep function. This is where you set it up. How long in Ms to stay
// awake, how long in Ms to dim down the screen as a warning.
void sleepMgr::setupSleep(int awakeMS,int rampDownMs) {

   shutOffSleep();                              // Just in case its already running..
   awakeTimer     = new timeObj(awakeMS);       // Create a timer for awake time.
   rampDownTimer  = new timeObj(rampDownMs);    // Create a timer for going dark.
   wakeup();                                    // Make sure we start it up.
}


// No longer want to use the sleep function? This will shut it down.
void sleepMgr::shutOffSleep(void) {

   if (awakeTimer) {          // if we have an awake timer..
      delete(awakeTimer);     // delete the awake timer..
      awakeTimer = NULL;      // Set it to NULL as a flag.
   }
   if (rampDownTimer) {       // if we have a ramp down timer..
      delete(rampDownTimer);  // delete the ramp down timer..
      rampDownTimer = NULL;   // Set it to NULL as a flag.
   }
   sleepState = awake;        // From now on, we are awake.
}


// Meant to be called repeatedly. This keeps track of the state of the sleeping function
// and returns the fraction of "Awake" your screen is.
float sleepMgr::checkSleep(void) {
   
   if (awakeTimer && rampDownTimer) {                          // If we have both timers..
      switch (sleepState) {                                    // Check our sleep sate..
         case awake :                                          // If we are awake..
            if (awakeTimer->ding()) {                          // If the awake timer has expired..
               rampDownTimer->start();                         // We begin the ramp down period
               sleepState = rampDown;                          // And note we are ramping down.
               return 100 * (1-rampDownTimer->getFraction());  // rampDown fraction left.
            }
            return 0;                                          // We are still awake, no dimming at all.
         case rampDown :                                       // Check our ramp down state..
            if (rampDownTimer->ding()) {                       // If the ramp down timer has expired..
               sleepState = sleeping;                          // We are in the sleeping state.
               return 100;                                     // Sleeping would be 100% darken.
            }
            return 100 * (1-rampDownTimer->getFraction());     // rampDown fraction left.
         case sleeping : return 100;                           // Sleeping would be 100% darken.
      }
   } else {                                                    // Else we DON'T have both timers..
      sleepState     = awake;                                  // In this case, we are awake.
      return 0;                                                // Therefor, no dimming at all.
   }
   return 0;                                                   // No way we can get here but.. 0 is default.
}
   

// When something happens that should wake up the machine, call this to wake it up.
void sleepMgr::wakeup(void) {

   if (awakeTimer && rampDownTimer) {           // If we have both timers..
      awakeTimer->start();                      // Start up the awake timer.
   }
   sleepState     = awake;                      // In any case, we are awake now.
}

sleepMgr ourSleepMgr;
