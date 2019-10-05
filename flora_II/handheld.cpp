#include "handheld.h"
#include "parameters.h"
#include "pumpObj.h"
#include "UI.h"
#include "globals.h"

#define COM_BUFF_BYTES  255 // Can be no larger than 255!


enum floraComSet  {
                  floraReset,
                  
                  readName,
                  setName,
                  readDryLimit,
                  setDryLimit,
                  readWaterTime,
                  setWaterTime,
                  readSoakTime,
                  setSoakTime,
                  
                  readPulse,
                  pulseOn,
                  pulseOff,
                  
                  readPump,
                  pumpOn,
                  pumpOff,

                  readLogging,
                  loggingOn,
                  loggingOff,
                  readLogSize,
                  readLogLines,
                  readLogWLines,
                  clearLog,
                  readLogBuff,
             
                  readState,
                  readTemp,
                  readMoisture
                  };

                      
enum floraReplySet  { noErr, unknownCom, badParam, unknownErr };

byte  comBuff[COM_BUFF_BYTES];              // Buffer for comunication.


handheld ourHandheld;


handheld::handheld(void) {  }


handheld::~handheld(void) {  }


void handheld::begin(void) { qCSlave::begin(comBuff, COM_BUFF_BYTES, 115200); }


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
      case readPump       : handleReadPump(comPtr);       break;

      case pumpOn         : 
      case pumpOff        : handleSetPump(comPtr);        break;
      case setName        : handleSetName(comPtr);        break;
      case setDryLimit    : handleSetDryLimit(comPtr);    break;
      case setWaterTime   : handleSetWaterTime(comPtr);   break;
      case setSoakTime    : handleSetSoakTime(comPtr);    break;
      case pulseOn        :
      case pulseOff       : handleSetPulseOnOff(comPtr);  break;

      case readLogging    : handleGetLogState(comPtr);    break;
      case loggingOn      : 
      case loggingOff     : handleSetLogState(comPtr);    break;
      case readLogSize    : handleGetLogSize(comPtr);     break;
      case readLogLines   : handleGetLogLines(comPtr);    break;
      case readLogWLines  : handleGetLogWLines(comPtr);   break;
      case readLogBuff    : handleGetLogBuff(comPtr);     break;
      case clearLog       : handleDeleteLogFile(comPtr);  break;
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
  
  comPtr[0] = (byte)weAre;
  replyComBuff(1);
}


void handheld::handleReadTemp(byte* comPtr) {

  comPtr[0] = (byte)round(tempC);
  replyComBuff(1);
}


void handheld::handleReadMoisture(byte* comPtr) {

  comPtr[0] = (byte)round(moisture);
  replyComBuff(1);
}


void handheld::handleReadDryLimit(byte* comPtr) {

  comPtr[0] = (byte)ourParamObj.getDryLimit();
  replyComBuff(1);
}


void handheld::handleReadWTime(byte* comPtr) {

  long* lPtr;
  
  lPtr = (long*)comPtr;               // We set up a long pointer to point to the buffer.
  *lPtr = ourParamObj.getWaterTime(); // Stuff the value into that long pointer location (The buffer).
  replyComBuff(sizeof(long));         // Send it on its way with the number of bytes we are sending.
}


void handheld::handleReadSTime(byte* comPtr) {

  long*  lPtr;
  
  lPtr = (long*)comPtr;
  *lPtr = ourParamObj.getSoakTime();
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


void handheld::handleSetName(byte* comPtr) {

  ourParamObj.setName(&comPtr[1]);
  comPtr[0] = noErr;
  replyComBuff(1);
}


void handheld::handleReadPump(byte* comPtr) {

  comPtr[0] = (byte)ourPump.pumpOn();
  replyComBuff(1);
}


void handheld::handleSetPump(byte* comPtr) {

  pumpCom = false;          // defautlt to pump off.
  if (comPtr[0]==pumpOn) {  // If they say they want it on..
    pumpCom = true;         // We turn it on.
  }
  comPtr[0] = noErr;        // No errors!
  replyComBuff(1);          // Send a reciept.
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

    
void handheld::handleGetLogState(byte* comPtr) {

  comPtr[0] = (byte) ourDisplay.isLogging();
  replyComBuff(1);
}


void handheld::handleSetLogState(byte* comPtr) {

  bool  success;
  
  if (comPtr[0]==loggingOn) {             // The first byte tells us on or off.    
    ourDisplay.setLogging(true);
    success = ourDisplay.isLogging();
  } else if (comPtr[0]==loggingOff) {
    ourDisplay.setLogging(false);
    success = !ourDisplay.isLogging();
  }
  if (success)  {
    comPtr[0] = 0;
  } else {
    comPtr[0] = unknownErr;
  }
  replyComBuff(1);
}


void handheld::handleGetLogSize(byte* comPtr) {

  unsigned long*  longPtr;
  
  longPtr = (unsigned long*)comPtr;         // Get the unsigned long pointer to point at our buffer.
  *longPtr = ourDisplay.getFileSize();      // Write the unsigned long out to the buffer.
  replyComBuff(sizeof(unsigned long));      // Send it on its way!
}


void handheld::handleGetLogLines(byte* comPtr) {

  unsigned long*  longPtr;
  
  longPtr = (unsigned long*)comPtr;         // Get the unsigned long pointer to point at our buffer.
  *longPtr = ourDisplay.getFileNumLines();  // Write the unsigned long out to the buffer.
  replyComBuff(sizeof(unsigned long));      // Send it on its way!
}

void handheld::handleGetLogWLines(byte* comPtr) {

  unsigned long*  longPtr;
  
  longPtr = (unsigned long*)comPtr;         // Get the unsigned long pointer to point at our buffer.
  *longPtr = ourDisplay.getFileNumWLines(); // Write the unsigned long out to the buffer.
  replyComBuff(sizeof(unsigned long));      // Send it on its way!
}

void handheld::handleGetLogBuff(byte* comPtr) {

  unsigned long   fileIndex;
  unsigned long   replyBytes;
  unsigned long*  longPtr;
  byte            maxBytes;

  maxBytes = comPtr[1];
  longPtr = (unsigned long*)&comPtr[2];                             // Our index is stored starting at index 1.
  fileIndex = *longPtr;                                             // read out the file index.
  Serial.print("maxBytes ");Serial.println(maxBytes);
  Serial.print("fileIndex ");Serial.println(fileIndex);
  replyBytes = ourDisplay.getFileBuff(fileIndex,maxBytes,comPtr);   // Call to fill the buffer.
  Serial.print("reply bytes ");Serial.println(replyBytes);
  replyComBuff(replyBytes);                                         // Send it on its way!
}


void handheld::handleDeleteLogFile(byte* comPtr) {

  ourDisplay.deleteLog();
  comPtr[0] = noErr;
  replyComBuff(1);
}
