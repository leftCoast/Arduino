#include "handheld.h"
#include "parameters.h"
#include "pumpObj.h"
#include "globals.h"

#define COM_BUFF_BYTES  255


enum floraComSet    { floraReset, readMoisture, readParams, pumpOn, pumpOff, setMoisture, setWaterTime, setSoakTime, setPulseOn, setPulsePeriod };
enum floraReplySet  { noErr, unknownCom, badParam };

byte  comBuff[COM_BUFF_BYTES];              // Buffer for comunication.


handheld ourHandheld;


handheld::handheld(void) {  }


handheld::~handheld(void) {  }


void handheld::begin(void) { qCSlave::begin(comBuff, COM_BUFF_BYTES, 9600); }


// Every time though the loop() we need to see if there's a command from the handheld controller
// waiting for us. If so, we need to handle it. This is what we do here.
void handheld::checkComs(void) {

  byte*     comPtr;
  
  if (haveBuff()) {                                     // If we have a complete command..
    Serial.println("Got command!");
    comPtr = getComBuff();                              // Point at the command character.
    switch (comPtr[0]) {                                          // First byte is our command. (Agreed on between us and the handheld.)
      case floraReset     : handleReset(comPtr);          break;  // The things we can do.. We do.
      case readMoisture   : handleReadMoisture(comPtr);   break;
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
        replyComBuff(1);                                          // And send that one byte on its way back to the handheld.
      break;
    }
  }
}


void handheld::handleReset(byte* comPtr) {

  ourParamObj.floraReset();
  comPtr[0] = noErr;
  replyComBuff(1);
}


void handheld::handleReadMoisture(byte* comPtr) {

  byte  moistureReading;

  moistureReading = round(moisture);
  Serial.println(moistureReading);
  comPtr[0] = moistureReading;
  comPtr[1] = noErr;
  replyComBuff(1);
}


void handheld::handleReadParams(byte* comPtr) {

  ourParamObj.readParams();
  comPtr[0] = noErr;
  replyComBuff(1); 
}

              
void handheld::handleSetPump(byte* comPtr) {

  pumpCom = false;
  if (comPtr[0]==pumpOn) {
    pumpCom = true;
  }
  comPtr[0] = noErr;
  replyComBuff(1);
}


void handheld::handleSetMoisture(byte* comPtr) {

  int newVal;
  
  newVal = *((int*)&(comPtr[1])); // Grab the value from the buffer.
  ourParamObj.setMoisture(newVal);
  comPtr[0] = noErr;
  replyComBuff(1);
}


void handheld::handleSetWaterTime(byte* comPtr){

  
}


void handheld::handleSetSoakTime(byte* comPtr){

  
}


void handheld::handleSetPulseOn(byte* comPtr){

  
}


void handheld::handleSetPulsePeriod(byte* comPtr){


}
