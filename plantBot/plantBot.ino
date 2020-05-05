


#define ANALOG_PIN      A4
#define DEF_CSMOOTH     20
#define DEF_WTIME    20000.0
#define DEF_STIME    60000.0
#define READ_TIME     1000.0
#define DEF_DRY
#define DEF_MUD


enum        weDo { sitting, autoWater, manualWater, soaking }; // Possible states.
weDo        weAre;                                             // Current state.
int         moisture;                                          // Calculated moisture %
runningAvg  capSmooth(DEF_CSMOOTHER);                          // Running avarage for the raw capacitive readings.
timeObj     waterTime(DEF_WTIME);                              // Time length to water when plant shows dry. (ms)
timeObj     soakTime(DEF_STIME);                               // Time to wait after watering before going back to watching mosture level.
timeObj     readTime.(READ_TIME);
mapper      mudMapper(DEF_DRY,DEF_MUD,0,100);

void setup() {
  
   ourPump.begin();                                      // Init pump.
   ourDisplay.begin();                                   // Init display.
   ourHandheld.begin();                                  // Setup coms for the handheld controller.
   Serial.print("ourHandheld result (0 is good) : ");
   Serial.println(ourHandheld.readErr());

   weAre = soaking;                                      // Our state is soaking. This gives things time to settle out.
   soakTime.start();                                     // And we start up the soak timer for that time.
   readTime.start();                                     // Fire up the read timer. Kinda' the heartbeat of this thing.
}


void checkHardware(void) {
   
   if (readTime.ding()) {                                   // If its time to do a reading..
      capread = analogRead(ANALOG_PIN);                     // Well, do it.
      capread = capSmooth.addData(capread);                 // Pop the capacitive value into the capacitive smoother. (Running avarage)
      moisture = round(mudMapper.Map(capread));             // Map the resulting capacitive value to a percent.
      readTime.stepTime();                                  // Reset the timer for the next reading.
   }
}


void loop() {

   uint16_t  capread;
   
   idle();                                                  // As always, let the idlers do their thing.
   checkHardware();                                         // Check hardware, note readings.
   ourHandheld.checkComs();                                 // Give handheld some time.
   switch (weAre) {                                         // OK, state stuff. Depending on our current state..
      case sitting :                                        // Sitting = watching moisture, wating to start watering.
         if (handheld.waterOn()) {                          // First, see if manual water on has been clicked.
            ourPump.setPump(true);                          // Fire up the pump.
            weAre = manualWater;                            // Set state that we are in manualWater mode.
         } else if (moisture < ourParamObj.getDryLimit()) { // Not doing manualWater and if its too dry? Time to water..
            setPump(true);                                  // Fire up the pump.
            waterTime.start();                              // Start up the watering timer.
            weAre = autoWater;                              // Set state that we are in watering mode.
         }
      break;
      case autoWater :                                      // autoWater = running the pump and watering plants on a timer.
         if (waterTime.ding()||waterOff()) {                // If our time for watering has expired, or water off was clicked.
            ourPump.setPump(false);                         // Shut the pump down.
            soakTime.start();                               // Start up the soaking timer.
            weAre = soaking;                                // Set state that we are in soaking mode.
         }
      break; 
      case manualWater :                                    // manualWater = running the pump and watering plants from handheld.
         if (handheld.waterOff()) {                         // If our time for watering has expired...
            ourPump.setPump(false);                         // Shut the pump down.
            soakTime.start();                               // Start up the soaking timer.
            weAre = soaking;                                // Set state that we are in soaking mode.
         }
      break;
      case soaking :                                        // soaking = waiting for the water to soak through the soil to the sensor.
         if (handheld.waterOn()) {                          // If manual water is on.
            ourPump.setPump(true);                          // Fire up the pump.
            weAre = manualWater;                            // Set state that we are in watering mode.
         } else if (soakTime.ding()) {                      // If soak time has expired..
            weAre = sitting;                                // Set state that we are in sitting mode.
         }
      break;                                                // All done, lets bolt!
   }
}
