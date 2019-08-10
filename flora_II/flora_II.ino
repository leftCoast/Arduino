#include <Adafruit_seesaw.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>
#include <quickCom.h>
#include <lilParser.h>

#include "parameters.h"
#include "pumpObj.h"
#include "textComObj.h"


#define DRY             315
#define MUD             1015
#define COM_BUFF_BYTES  255

enum floraComSet    { floraReset, readParams, pumpOn, pumpOff, setMoisture, setWaterTime, setSoakTime, setPulseOn, setPulsePeriod };
enum floraReplySet  { noErr, unknownCom, badParam };


Adafruit_seesaw ss;                               // The moisture sensor.

float       moisture;                             // The current moisture reading.

enum        weDo { sitting, watering, soaking };  // Possible states.
weDo        weAre;                                // Current state.
blinker     idleLight(13,80,2000);                // Blinker saying we're running.

qCSlave     ourComObj;                            // Our object that comunicates with the handheld controller.
byte        comBuff[COM_BUFF_BYTES];              // Buffer for comunication.


timeObj*      waterTime = NULL;                   // Time length to water when plant shows dry. (ms)
timeObj*      soakTime = NULL;                    // Time to wait after watering before going back to watching mosture level.

runningAvg    cSmoother(DEF_CSMOOTHER);            // Running avarage for the raw capacitive readings.
runningAvg    tSmoother(DEF_TSMOOTHER);            // Running avarage for the raw tempature readings.
timeObj       readTime(DEF_READ_TIME);             // Time between moisture readings.
mapper        mudMapper(DRY,MUD,0,100);            // Mapper from raw capacitive reading to percent.


// Setup everything we can without a param set. Everything that is only done once.
// Then we read in the parameters and start up operation. The doReadParams() call is
// different in that it can be called randomly during the runtime of the machine.
void setup() {
  
  ourPump.setPump(false);                               // Off with the pump.
  
  Serial.begin(57600);                                  // Fire up serial port. (Debugging)
  Serial.println("Hello?");
    
  textComs.begin();                                     // Set up parser so we can talk to the computer.
  
  ourComObj.begin(comBuff, COM_BUFF_BYTES, 9600);       // Buff, bytes & Baud. Setup coms for the handheld controller.
  Serial.print("ourComObj result (0 is good) : ");
  Serial.println(ourComObj.readErr());
  
  delay(1000);                                          // Just in case its not ready, go have a cigarette. Then we'll have a go at firing it up.
  if (!ss.begin(0x36)) {                                // Start up moisture sensor.
    Serial.println("ERROR! no Sensor.");                // Failed!
    while(1);                                           // Lock here.
  }
  Serial.println("Prime running avarage buffers");
  for (int i=1;i<DEF_CSMOOTHER;i++) {
    doReading();
    delay(100);
  }
  
  ourParamObj.readParams();                             // Read our saved running params.
  updateEnv();                                          // Setup what we need to setup with params.
  weAre = sitting;                                      // Our state is sitting. (Watching moisture level)
  readTime.start();                                     // Fire up the read timer. We're live!

  idleLight.setBlink(true);                             // Start up our running light.
  Serial.println("Sytem ready."); 
}


// We have a fresh load of parameters. Initialze the machine with them,
void updateEnv(void) {

  if (waterTime) delete(waterTime);                                   // These three are for freeing memeory from the
  if (soakTime) delete(soakTime);                                     // Last set of parameters. We'll need to build up
  
  waterTime   = new timeObj(ourParamObj.getWaterTime());              // Create an object for tracking watering time.
  soakTime    = new timeObj(ourParamObj.getSoakTime());               // Create an object for tracking soak time.
  
  ourPump.setPercent(ourParamObj.getPWMPercent());
  ourPump.setPeriod(ourParamObj.getPWMPeriod());
  needReset = false;
}


// After all the nonsense in loop(). It all boils down to, do we want the pump on or not?
// Here's where we look at everything and do just that.
void doSetPump(void) {

  if (pumpCom||weAre==watering) {   // Currently two things tell us to water. pumpCom from the controller and our state.
    if (!ourPump.pumpOn()) {        // We want the pump on and its not on now?
      ourPump.setPump(true);        // Turn on the pump.
    }
  } else {                          // We want the pump off?
    ourPump.setPump(false);         // Off with the pump.
  }
}


// Every time though the loop() we need to see if there's a command from the handheld controller
// waiting for us. If so, we need to handle it. This is what we do here.
void checkComs(void) {

  byte*     comPtr;
  
  if (ourComObj.haveBuff()) {                                     // If we have a complete command..
    Serial.println(ourComObj.readErr());
    comPtr = ourComObj.getComBuff();                              // Point at the command character.
    Serial.println(comPtr[0]);
    switch (comPtr[0]) {                                          // First byte is our command. (Agreed on between us and the handheld.)
      case floraReset     : handleReset(comPtr);          break;  // The things we can do.. We do.
      case readParams     : handleReadParams(comPtr);     break;
      case pumpOn         :
      case pumpOff        : handleSetPump(comPtr);        break;
      case setMoisture    : handleSetMoisture(comPtr);    break;
      case setWaterTime   : handleSetWaterTime(comPtr);   break;
      case setSoakTime    : handleSetSoakTime(comPtr);    break;
      case setPulseOn     : handleSetPulseOn(comPtr);     break;
      case setPulsePeriod : handleSetPulsePeriod(comPtr); break;
      default :                                                   // If we can't we pass back and error. (What? I don't get it..)
        comPtr[0] = unknownCom;                                   // Stuff in "unknown command" reply byte.
        ourComObj.replyComBuff(1);                                // And send that one byte on its way back to the handheld.
      break;
    }
  }
}


// If your watching with the serial port on the computer, this'l give output to see what's up.
void debugDataOut(float tempC,uint16_t capread) {
  
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("*C   ");
    Serial.print("Capacitive: ");
    Serial.print(capread);Serial.print("   ");
    Serial.print("Moisture: ");
    Serial.print((int)moisture);
    Serial.print("%  current state : ");
    
    switch (weAre) {
      case sitting  : Serial.println("sitting");  break;
      case watering : Serial.println("watering"); break;
      case soaking  : Serial.println("soaking");  break;
    }
}

// Get the info from the sensor and refine it to the point we can use it.
void doReading(void) {

  float     tempC;
  uint16_t  capread;
  
  tempC = ss.getTemp();                   // Read the tempature from the sensor.
  capread = ss.touchRead(0);              // Read the capacitive value from the sensor.
  capread = cSmoother.addData(capread);   // Pop the capacitive value into the capacitive smoother. (Running avarage)
  tempC = tSmoother.addData(tempC);       // Pop the tempature value into the tempature smoother. (Again, running avarage)
  moisture = mudMapper.Map(capread);      // Map the resulting capacitive value to a percent.
  moisture = round(moisture);             // Round it to an int.
  debugDataOut(tempC,capread);            // Show the world, if its looking.
}


// Ah loop(). This is what we do all day.
void loop() {
  
  idle();                                             // Calling idle() first is always a good idea. Just in case there are idlers that need it.
  if (needReset) {                                    // If our params have changed..
    updateEnv();                                      // Update all the things they effect.
  }
  textComs.checkTextCom();                            // Check to see if theres a guy at the computer looking to talk to us.
  checkComs();                                        // Now we bop off and check to see if anything has come in from a handheld controller.
  if (readTime.ding()) {                              // If its tiome to do a reading..
    doReading();
    readTime.stepTime();                              // Reset the timer for the next reading.
  }

  switch (weAre) {                                    // OK state stuff. depending on our current state..
    case sitting :                                    // Sitting = watching moisture wating to start watering.
      if (moisture<ourParamObj.getMoisture()) {   // If its too dry and time to water..
        Serial.println("Watering");                   // Tell the world what's up next. (Again, if they are listening)
        waterTime->start();                           // Start up the watring timer.
        weAre = watering;                             // Set state that we are in watering mode.
      }
     break;                                           // All done, jet!
     case watering :                                  // Watering = running the pump and watering plants.
      if (waterTime->ding()) {                        // If our time for watering has expired...            
        Serial.println("Soaking");                    // Tell the world what's up next. (And again, if they are listening) 
        soakTime->start();                            // Start up the soaking timer.
        weAre = soaking;                              // Set state that we are in soaking mode.
      }
     break;                                           // That's it for now.
     case soaking :                                   // soaking = waiting for the water to soak through the soil to the sensor.
      if (soakTime->ding()) {                         // If soak time has expired..
        Serial.println("Back sitting.");              // Tell the world what's up next.
        weAre = sitting;                              // Set state that we are in sitting mode. 
      }
     break;                                           // All done, lets bolt!
  }
  doSetPump();                                        // Now that commands nd state have been checked, decide if the pump goes on or off.
}

    
// ******************************
// ********** Handlers **********
// ********** for the  **********
// ***** handheld commmands *****
// ******************************


void handleReset(byte* comPtr) {

  ourParamObj.floraReset();
  comPtr[0] = noErr;
  ourComObj.replyComBuff(1);
}


void handleReadParams(byte* comPtr) {

  ourParamObj.readParams();
  comPtr[0] = noErr;
  ourComObj.replyComBuff(1); 
}

              
void handleSetPump(byte* comPtr) {

  pumpCom = false;
  if (comPtr[0]==pumpOn) {
    pumpCom = true;
  }
  comPtr[0] = noErr;
  ourComObj.replyComBuff(1);
}


void handleSetMoisture(byte* comPtr) {

  int newVal;
  
  newVal = *((int*)&(comPtr[1])); // Grab the value from the buffer.
  ourParamObj.setMoisture(newVal);
  updateEnv();
  comPtr[0] = noErr;
  ourComObj.replyComBuff(1);
}


void handleSetWaterTime(byte* comPtr){

  
}


void handleSetSoakTime(byte* comPtr){

  
}


void handleSetPulseOn(byte* comPtr){

  
}


void handleSetPulsePeriod(byte* comPtr){


}
