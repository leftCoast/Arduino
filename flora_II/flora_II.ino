#include <Adafruit_seesaw.h>
#include <EEPROM.h>

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


#define MOTOR_1_PIN 16

#define DRY                     315
#define MUD                     1015
#define DEF_MOISTURE_LIMIT      45
#define DEF_READ_TIME           10000
#define DEF_WATER_TIME          10000
#define DEF_SOAK_TIME           120000
#define DEF_CSMOOTHER           20
#define DEF_TSMOOTHER           20
#define DEF_MOTORPULSE_ON       125
#define DEF_MOTORPULSE_PERIOD   250
#define DEF_NAME                "Plant name"
#define COM_BUFF_BYTES          255
#define NAME_BUFF_BYTES         24


// ******************************************
// *********   COPY TO CONTROLLER   *********
// ******************************************


enum floraComSet    { floraReset, readParams, pumpOn, pumpOff, setMoisture, setWaterTime, setSoakTime, setPulseOn, setPulsePeriod };
enum floraReplySet  { noErr, unknownCom, badParam };

struct paramType {
  int moisture;
  int waterTime;
  int soakTime;
  int pulse;
  int period;
  char name[NAME_BUFF_BYTES];
};


// ******************************************
// ******************************************
// ******************************************

Adafruit_seesaw ss;                               // The moisture sensor.

float       moisture;                             // The current moisture reading.
bool        motorOn;                              // Is the pump currently running?
bool        pumpCom;                              // Are we being told to turn the pump on by the controller?

enum        weDo { sitting, watering, soaking };  // Possible states.
weDo        weAre;                                // Current state.
blinker     sittingLight(13,80,4000);             // Red blinker saaying we're watching moisture.

qCSlave     ourComObj;                            // Our object that comunicates with the handheld controller.
byte        comBuff[COM_BUFF_BYTES];              // Buffer for comunication.

paramType     params;                             // Struct of operation parameters.
char*         plantName = NULL;                   // We save a plant name for the controller to display.
timeObj*      waterTime = NULL;                   // Time length to water when plant shows dry. (ms)
timeObj*      soakTime = NULL;                    // Time to wait after watering before going back to watching mosture level.
blinker*      motorPulse = NULL;                  // Object that controls the timing of pump pulsing. (Optional)

runningAvg   cSmoother(DEF_CSMOOTHER);            // Running avarage for the raw capacitive readings.
runningAvg   tSmoother(DEF_TSMOOTHER);            // Running avarage for the raw tempature readings.
timeObj      readTime(DEF_READ_TIME);             // Time between moisture readings.
mapper       mudMapper(DRY,MUD,0,100);            // Mapper from raw capacitive reading to percent.

lilParser mParser;
enum      commands { noCommand, resetAll, showParams, setSetPoint, setWTime, setSTime };


// Setup everything we can without a param set. Everything that is only done once.
// Then we read in the parameters and start up operation. The doReadParams() call is
// different in that it can be called randomly during the runtime of the machine.
void setup() {


  pinMode(MOTOR_1_PIN, OUTPUT);                   // IN1 = Pump
  digitalWrite(MOTOR_1_PIN, LOW);                 // Pump off.
  sittingLight.setLight(false);                   // Red LED blinker off.
  
  setUpParser();                                  // So we can talk to the computer.
  
  ourComObj.begin(comBuff, COM_BUFF_BYTES, 9600); // Buff, bytes & Baud. Setup coms for the handheld controller.

  Serial.begin(57600);                            // Fire up serial port. (Debugging)
  Serial.println("Hello?");
  
  if (!ss.begin(0x36)) {                          // Start up moisture sensor.
    Serial.println("ERROR! no Sensor.");          // Failed!
    while(1);                                     // Lock here.
  }
  
  doReadParams();                                 // Read our saved running params and start operation.

  Serial.println("Initial readings to setup system.");
  for (int i=1;i<DEF_CSMOOTHER;i++) {
    doReading();
    delay(250);
  }
  Serial.println("Sytem ready.");
    
}


// We have a fresh load of parameters. Initialze the machine with them,
void doInit(void) {

  if (waterTime) delete(waterTime);                                   // These three are for freeing memeory from the
  if (soakTime) delete(soakTime);                                     // Last set of parameters. We'll need to build up
  if (motorPulse) delete(motorPulse);                                 // a fresh set for these new parameters.
  
  waterTime   = new timeObj(params.waterTime);                        // Create an object for tracking watering time.
  soakTime    = new timeObj(params.soakTime);                         // Create an object for tracking soak time.
  motorPulse  = new blinker(MOTOR_1_PIN,params.pulse,params.period);  // Create an object for controlling the pump pulsing.
  
  weAre = sitting;                                                    // Our state is sitting. (Watching moisture level)
  sittingLight.setBlink(true);                                        // Turn on the red blinking LED.
  motorOn = false;                                                    // Pump is NOT on.
  pumpCom = false;                                                    // No one has told us to turn it on.
  readTime.start();                                                   // Fire up the read timer. We're live!
}


// Link command bytes to text sstrings that can be typed.
void setUpParser() {

  mParser.addCmd(resetAll, "reset");
  mParser.addCmd(showParams, "see");
  mParser.addCmd(setSetPoint, "moisture");
  mParser.addCmd(setWTime, "wtime"); 
  mParser.addCmd(setSTime, "stime");
  //mParser.addCmd(setSTime, "pump");
  mParser.addCmd(setSTime, "name");
}


// After all the nonsense in loop(). It all boils down to, do we want the pump on or not?
// Here's where we look at everything and do just that.
void setPump(void) {

  if (pumpCom||weAre==watering) {   // Currently two things tell us to water. pumpCom from the controller and our state.
    if (!motorOn) {                 // We want the pump on and its not on now?
      motorPulse->setBlink(true);    // Turn on the motor pulse thing. This controlls the pump.
      motorOn = true;               // Note that we turned it on.
    }
  } else {                          // We want the pump off?
    motorPulse->setBlink(false);     // Easy peasy! Just shut down the pulse thing.
    motorOn = false;                // Note that the pump's off.
  }
}


// We want to save the param set we are using right now.
void doWriteParams(void) { EEPROM.put(0,params); }


// From any state we want to reset to the default parameters. (Goood for first fire ups)
void doResetParams(void) {  
                
  params.moisture   = DEF_MOISTURE_LIMIT;     // Default numbers.
  params.waterTime  = DEF_WATER_TIME;
  params.soakTime   = DEF_SOAK_TIME;
  params.pulse      = DEF_MOTORPULSE_ON;
  params.period     = DEF_MOTORPULSE_PERIOD;
  strcpy(params.name,DEF_NAME);               // Our default name.
  doWriteParams();                            // Put these values out to "disk"                              
  doInit();                                     // Init everything using these values.
}


// From any state lets read in a fresh set of parameters and (re)start operation.
void doReadParams(void) {

  EEPROM.get(0,params);   // Grab the set of params out of storage.
  doInit();               // (re)start operation.
}


void checkCompComs(void) {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {                       
      case noCommand    : break;
      case resetAll     : doResetParams(); break;
      case showParams   : doShowParams(); break;
      case setSetPoint  : handelCompSetPoint(); break;
      case setWTime     : handelCompSetWTime(); break;
      case setSTime     : handelCompSetSTime(); break;
      default           : Serial.println("Sorry, have no idea what you want.");
    }
    if (command) Serial.print(">");
  }
}


// Every time though the loop() we need to see if there's a command from the handheld controller
// waiting for us. If so, we need to handle it. This is what we do here.
void checkComs(void) {

  byte*     comPtr;
  
  if (ourComObj.haveBuff()) {                                     // If we have a complete command..
    comPtr = ourComObj.getComBuff();                              // Point at the command character.
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

// Get the infor from the sensor and refine it to the point we can use it.
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
  
  idle();                                   // Calling idle() first is always a good idea. Just in case there are idlers that need it.
  checkCompComs();                          // Check to see if theres a guy at the computer looking to talk to us.
  checkComs();                              // Now we bop off and check to see if anything has come in from a handheld controller.
  if (readTime.ding()) {                    // If its tiome to do a reading..
    doReading();
    readTime.stepTime();                    // Reset the timer for the next reading.
  }

  switch (weAre) {                          // OK state stuff. depending on our current state..
    case sitting :                          // Sitting = watching moisture wating to start watering.
      if (moisture<params.moisture) {       // If its too dry and time to water..
        Serial.println("Watering");         // Tell the world what's up next. (Again, if they are listening)
        waterTime->start();                 // Start up the watring timer.
        sittingLight.setBlink(false);       // Shut off the blinking "I'm watching" LED.
        weAre = watering;                   // Set state that we are in watering mode.
      }
     break;                                 // All done, jet!
     case watering :                        // Watering = running the pump and watering plants.
      if (waterTime->ding()) {              // If our time for watering has expired...            
        Serial.println("Soaking");          // Tell the world what's up next. (And again, if they are listening) 
        soakTime->start();                  // Start up the soaking timer.
        weAre = soaking;                    // Set state that we are in soaking mode.
      }
     break;                                 // That's it for now.
     case soaking :                         // soaking = waiting for the water to soak through the soil to the sensor.
      if (soakTime->ding()) {               // If soak time has expired..
        Serial.println("Back sitting.");    // Tell the world what's up next.
        sittingLight.setBlink(true);        // Turn the sitting blinker back on.
        weAre = sitting;                    // Set state that we are in sitting mode. 
      }
     break;                                 // All done, lets bolt!
  }
  setPump();                                // Now that commands nd state have been checked, decide if the pump goes on or off.
}


// ******************************
// ********** Handlers **********
// ********** for the  **********
// ***** computer commmands *****
// ******************************


void doShowParams(void) {

  Serial.print("Parameters for ");Serial.println(plantName);
  Serial.print("moisture  : ");Serial.println(params.moisture);
  Serial.print("waterTime : ");Serial.println(params.waterTime);
  Serial.print("soakTime  : ");Serial.println(params.soakTime);
  Serial.print("pulse     : ");Serial.println(params.pulse);
  Serial.print("period    : ");Serial.println(params.period);
  Serial.print("name      : ");Serial.println(params.name);
  Serial.println();
}


void handelCompSetPoint(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    if (newVal<0) newVal = 0;
    if (newVal>100) newVal = 100;
    params.moisture = newVal;
    doWriteParams();
    doInit();
    Serial.print("Moisture set point now set to ");
    Serial.println(params.moisture);
  }
}


void handelCompSetWTime(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    if (newVal<0) newVal = 0;
    params.waterTime = newVal;
    doWriteParams();
    doInit();
    Serial.print("Watering tmie now set to ");
    Serial.println(params.waterTime);
  }
}


void handelCompSetSTime(void) {

  int   newVal;
  char* paramBuff;
  
  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    if (newVal<0) newVal = 0;
    params.soakTime = newVal;
    doWriteParams();
    doInit();
    Serial.print("Soak time now set to ");
    Serial.println(params.soakTime);
  }
}



    
// ******************************
// ********** Handlers **********
// ********** for the  **********
// ***** handheld commmands *****
// ******************************


void handleReset(byte* comPtr) {

  doResetParams();
  comPtr[0] = noErr;
  ourComObj.replyComBuff(1);
}


void handleReadParams(byte* comPtr) {

  doReadParams();
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
  if (newVal>=0&&newVal<=100) {
    params.moisture = newVal;
    doWriteParams();
    doInit();
    comPtr[0] = noErr;
  } else {
    comPtr[0] = badParam;
  }
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
