#include "handheld.h"
#include "parameters.h"
#include "pumpObj.h"
#include "globals.h"

#define COM_BUFF_BYTES  255


enum floraComSet    { floraReset, readName, readState, readMoisture, readTemp, readDryLimit, readWaterTime, readSoakTime, pumpOn, pumpOff, setDryLimit, setWaterTime, setSoakTime, setPulseOn, setPulseOff };
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
    comPtr = getComBuff();                              // Point at the command character.
    //Serial.print("Recieved command # ");
    //Serial.println((byte)comPtr[0]);
    switch (comPtr[0]) {                                          // First byte is our command. (Agreed on between us and the handheld.)
      case floraReset     : handleReset(comPtr);          break;  // The things we can do.. We do.
      case readState      : handleReadState(comPtr);      break;
      case readTemp       : handleReadTemp(comPtr);       break;
      case readMoisture   : handleReadMoisture(comPtr);   break;
      case readDryLimit   : handleReadDryLimit(comPtr);   break;
      case readWaterTime  : handleReadWTime(comPtr);      break;
      case readSoakTime   : handleReadSTime(comPtr);      break;
      case readName       : handleReadName(comPtr);       break;
      case pumpOn         :
      case pumpOff        : handleSetPump(comPtr);        break;
      case setDryLimit    : handleSetDryLimit(comPtr);    break;
      case setWaterTime   : handleSetWaterTime(comPtr);   break;
      case setSoakTime    : handleSetSoakTime(comPtr);    break;
      case setPulseOn     :
      case setPulseOff    : handleSetPulseOnOff(comPtr);  break;
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


void handheld::handleReadState(byte* comPtr) {
  
  byte  stateByte;

  stateByte = (byte)weAre;
  comPtr[0] = stateByte;
  comPtr[1] = noErr;
  replyComBuff(1);
}


void handheld::handleReadTemp(byte* comPtr) {

  byte  tempByte;

  tempByte = round(tempC);
  comPtr[0] = tempByte;
  comPtr[1] = noErr;
  replyComBuff(1);
}


void handheld::handleReadMoisture(byte* comPtr) {

  byte  moistureReading;

  moistureReading = round(moisture);
  comPtr[0] = moistureReading;
  comPtr[1] = noErr;
  replyComBuff(1);
}


void handheld::handleReadDryLimit(byte* comPtr) {

  byte  dryLimit;

  dryLimit = ourParamObj.getDryLimit();
  comPtr[0] = dryLimit;
  comPtr[1] = noErr;
  replyComBuff(1);
}


void handheld::handleReadWTime(byte* comPtr) {

  long  waterTime;
  long* lPtr;
  
  waterTime = ourParamObj.getWaterTime();   // We grab the actual value.
  lPtr = (long*)&(comPtr[0]);               // We set up a long pointer to point to the buffer.
  *lPtr = waterTime;                        // Stuff the value into that long pointer location (The buffer).
  replyComBuff(sizeof(long));               // Send it on its way with the number of bytes we are sending.
}


void handheld::handleReadSTime(byte* comPtr) {

  long   soakTime;
  long*  lPtr;
  
  soakTime = ourParamObj.getSoakTime();
  lPtr = (long*)&(comPtr[0]);
  *lPtr = soakTime;
  replyComBuff(sizeof(long));
}


void handheld::handleReadName(byte* comPtr) {

  char* name;
  char* charPtr;
  
  name = ourParamObj.getName();
  charPtr = (char*)&(comPtr[0]);
  strcpy(charPtr,name);
  replyComBuff(strlen(name)+1);
}


void handheld::handleSetPump(byte* comPtr) {

  Serial.print("Recieved command # ");
  Serial.println((byte)comPtr[0]);
  pumpCom = false;
  if (comPtr[0]==pumpOn) {
    pumpCom = true;
    Serial.println("Pump on!");
  } else {
    Serial.println("Pump off!");
  }
  comPtr[0] = noErr;
  replyComBuff(1);
  
}


void handheld::handleSetDryLimit(byte* comPtr) {

  byte newVal;
  
  newVal = *((byte*)&(comPtr[1])); // Grab the value from the buffer.
  ourParamObj.setDryLimit(newVal);
  comPtr[0] = noErr;
  replyComBuff(1);
}


void handheld::handleSetWaterTime(byte* comPtr){

  long newVal;
  
  newVal = *((long*)&(comPtr[1])); // Grab the value from the buffer.
  ourParamObj.setWaterTime(newVal);
  comPtr[0] = noErr;
  replyComBuff(1);
}


void handheld::handleSetSoakTime(byte* comPtr){

  long newVal;
  
  newVal = *((long*)&(comPtr[1])); // Grab the value from the buffer.
  ourParamObj.setSoakTime(newVal);
  comPtr[0] = noErr;
  replyComBuff(1);
}


void handheld::handleSetPulseOnOff(byte* comPtr) {

  
}
