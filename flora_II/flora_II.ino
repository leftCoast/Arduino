
#include <blinker.h>
#include <colorObj.h>  
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>
#include <quickCom.h>
#include <lilParser.h>


#include "parameters.h"
#include "pumpObj.h"
#include "textComObj.h"
#include "handheld.h"
#include "globals.h"
#include "UI.h"



#define   ANALOG_PIN  A4


mapper*       mudMapper = NULL;        // Mapper from raw capacitive reading to percent.

runningAvg  cSmoother(DEF_CSMOOTHER);  // Running avarage for the raw capacitive readings.
timeObj     readTime(DEF_READ_TIME);   // Time between moisture readings.


// ********************************************************************
// ********************************************************************
// ********************************************************************
// Flipping the screen.

// Normal
//0xA8 = 64;
//0xA0 A[4] = 0;
//0xA2 = 0;
//0xA1 = 0;

// Inverse?
//0xA8 = 64;
//0xA0 A[4] = 1;
//0xA2 = 0;
//0xA1 = 0;



// ********************************************************************
// ********************************************************************
// ********************************************************************



// Setup everything we can without a param set. Everything that is only done once.
// Then we read in the parameters and start up operation. The doReadParams() call is
// different in that it can be called randomly during the runtime of the machine.
void setup() {

   ourPump.setPump(false);
   // Off with the pump. This must be called first to lock down control of the pump.
   // Otherwise, if say.. You find you have a dead sensor and the progam locks here..
   // The pump will randomly activate. It pickes up stray currents on the line that
   // trigger the driver hardware. Once this is called, the pump contol is locked in
   // and your good to go.
   
   pumpCom = false;                                      // The user is NOT telling us to turn on the pump right now.
   Serial.begin(57600);                                  // Fire up serial port. (Debugging)
   waterTime = NULL;                                     // Time length to water when plant shows dry. (ms)
   soakTime = NULL;                                      // Time to wait after watering before going back to watching mosture level.
   mudMapper = NULL;                                     // Mapping cap reading to percent moisture.
   ourParamObj.readParams();                             // Read our saved running params.
   updateEnv();                                          // Setup what we need to setup with params.
   weAre = soaking;                                      // We are going to be in soak to start. BUT, this is only for the UI to see during begin().
   readTime.start();                                     // Fire up the read timer. We're live!
   ourDisplay.begin();                                   // Fire up our "human interface". (fancy!)
   textComs.begin();                                     // Set up parser so we can talk to the computer.   
   ourHandheld.begin();                                  // Setup coms for the handheld controller.
}


// We have a fresh load of parameters. Initialze the machine with them
void updateEnv(void) {
      
  if (waterTime) delete(waterTime);                                              // These three are for freeing memeory from the
  if (soakTime) delete(soakTime);                                                // Last set of parameters. We'll need to build up
  if (mudMapper) delete(mudMapper);                                              // a new set for doing all the math, timing n stuff.
  waterTime   = new timeObj(ourParamObj.getWaterTime());                         // Create an object for tracking watering time.
  soakTime    = new timeObj(ourParamObj.getSoakTime());                          // Create an object for tracking soak time.
  mudMapper   = new mapper(ourParamObj.getDry(), ourParamObj.getMud(), 0, 100);  // Create the mapper to calculate moiture percent.
  ourPump.setPercent(ourParamObj.getPWMPercent());                               // How much power to run through the pump.
  ourPump.setPeriod(ourParamObj.getPWMPeriod());                                 // How smooth the pump's power switching is.
  needReset = false;                                                             // Note that we have fresh parameters.
  weAre = soaking;                                                               // Everything has changed? We go into soak to let things settle.
  soakTime->start();                                                             // Fire off the soak timer.
}


// After all the nonsense in loop(). It all boils down to, do we want the pump on or not?
// Here's where we look at everything and do just that.
void doSetPump(void) {

  if (pumpCom || weAre == watering) {  // Currently two things tell us to water. pumpCom from the controller and our state.
    if (!ourPump.pumpOn()) {           // We want the pump on and its not on now?
      ourPump.setPump(true);           // Turn on the pump.
    }
  } else {                             // We want the pump off?
    ourPump.setPump(false);            // Off with the pump.
  }
  if (ourPump.pumpOn()) {              // So after all that, if the pump is on..
    ourPump.setSpeed();                // We set its speed.
  }
}


// Get the info from the sensor and refine it to the point we can use it.
void doReading(void) {

  capread = analogRead(ANALOG_PIN);

  ourDisplay.addMode(weAre);
  ourDisplay.addRawCap(capread);

  capread = cSmoother.addData(capread);     // Pop the capacitive value into the capacitive smoother. (Running avarage)
  moisture = mudMapper->Map(capread);       // Map the resulting capacitive value to a percent.
  moisture = round(moisture);               // Round it to an int.

  ourDisplay.addAveCap(capread);
  ourDisplay.addMoisture(moisture);

  ourDisplay.addLimit(ourParamObj.getDryLimit());
  ourDisplay.addWaterSec(round(ourParamObj.getWaterTime() / 1000.0));
  ourDisplay.addSoakSec(round(ourParamObj.getSoakTime() / 1000.0));
  ourDisplay.saveDataRecord();
}


// Ah loop(). This is what we do all day.
void loop() {

  idle();                                             // Calling idle() first is always a good idea. Just in case there are idlers that need it.
  
  if (needReset) {                                    // If our params have changed..
    updateEnv();                                      // Update all the things they effect.
  }
  textComs.checkTextCom();                            // Check to see if theres a guy at the computer looking to talk to us.
  ourHandheld.checkComs();                            // Now we bop off and check to see if anything has come in from a handheld controller.

  if (readTime.ding()) {                              // If its time to do a reading..
    doReading();                                      // Well, do it.
    readTime.stepTime();                              // Reset the timer for the next reading.
  }

  switch (weAre) {                                    // OK, state stuff. Depending on our current state..
    case sitting :                                    // Sitting = watching moisture wating to start watering.
      if (moisture < ourParamObj.getDryLimit()) {     // If its too dry? Time to water..
        waterTime->start();                           // Start up the watering timer.
        weAre = watering;                             // Set state that we are in watering mode.
      }
      break;                                          // All done, jet!
    case watering :                                   // Watering = running the pump and watering plants.
      if (waterTime->ding()) {                        // If our time for watering has expired...
        soakTime->start();                            // Start up the soaking timer.
        weAre = soaking;                              // Set state that we are in soaking mode.
      }
      break;                                          // That's it for now.
    case soaking :                                    // soaking = waiting for the water to soak through the soil to the sensor.
      if (soakTime->ding()) {                         // If soak time has expired..
        weAre = sitting;                              // Set state that we are in sitting mode.
      }
      break;                                          // All done, lets bolt!
  }
  doSetPump();                                        // Now that commands and state have been checked, decide if the pump goes on or off.
}
