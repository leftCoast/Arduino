#include "handheld.h"
#include "parameters.h"
#include "pumpObj.h"
#include "UI.h"
#include "globals.h"
#include "textComObj.h"

#define COM_BUFF_BYTES  255 // Can be no larger than 255!
#define SLAVE_BAUD  9600 //115200 //500000

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
                  readTemp,         // Leave this here to keep synk with the handheld. (For now)
                  readMoisture,

                  textCom
                  };

                      
enum floraReplySet  { noErr, unknownCom, badParam, unknownErr };

byte  comBuff[COM_BUFF_BYTES];              // Buffer for comunication.


handheld ourHandheld;


handheld::handheld(void) {  }


handheld::~handheld(void) {  }


void handheld::begin(void) { qCSlave::begin(comBuff, COM_BUFF_BYTES, SLAVE_BAUD); }


// Every time though the loop() we need to see if there's a command from the handheld controller
// waiting for us. If so, we need to handle it. This is what we do here.
void handheld::checkComs(void) {

  byte*     comPtr;

  if (haveBuff()) {                                               // If we have a complete command..
    comPtr = getComBuff();                                        // Point at the command character.
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

      case textCom        : handleTextCom(comPtr);        break;
      default :                                                   // If we can't, we pass back an error. (What? I don't get it..)
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

  comPtr[0] = '\0';  // There is no longer a temp to read.
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

  ourParamObj.setName((char*)&comPtr[1]);
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


// This is for pulseing the water for a sprinkler or just
// flooding it out a pipe. But.. Never got around to all
// that now did I? It always pulese for now.
void handheld::handleSetPulseOnOff(byte* comPtr) { }

    
void handheld::handleGetLogState(byte* comPtr) {

  comPtr[0] = (byte) ourDisplay.isLogging();
  replyComBuff(1);
}


void handheld::handleSetLogState(byte* comPtr) {

  bool  success;

  success = false;
  if (comPtr[0]==loggingOn) {             // If the first byte says to turn on logging..    
    ourDisplay.setLogging(true);          // Fire up logging.
    success = ourDisplay.isLogging();     // See if it took.
  } else if (comPtr[0]==loggingOff) {     // Else, if the first byte says to turn logging off..
    ourDisplay.setLogging(false);         // SHut down logging.
    success = !ourDisplay.isLogging();    // See if it took.
  }
  if (success)  {                         // If it did what they wanted..
    comPtr[0] = 0;                        // Set up to reply 0 for no error.
  } else {                                // Else, it didn't do what they wanted..
    comPtr[0] = unknownErr;               // Set up to reply an error.
  }
  replyComBuff(1);                        // And back goes the reply.
}


void handheld::handleGetLogSize(byte* comPtr) {

  unsigned long*  longPtr;
  
  longPtr = (unsigned long*)comPtr;         // Get the unsigned long pointer to point at our buffer.
  *longPtr = ourDisplay.getFileSize();      // Write the unsigned long out to the buffer. (This gets byte order right and stuff.)
  replyComBuff(sizeof(unsigned long));      // Send it on its way!
}


void handheld::handleGetLogLines(byte* comPtr) {

  unsigned long*  longPtr;
  
  longPtr = (unsigned long*)comPtr;         // Get the unsigned long pointer to point at our buffer.
  *longPtr = ourDisplay.getFileNumLines();  // Write the unsigned long out to the buffer. (Again, this gets byte order right and stuff.)
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


// This one is different. Its basically saying we got a command string, let
// TextComObj deal with it. When it's done, we'll pretend it just didn't
// happen and go on with our lives. Everything, including the master on the 
// other end must block 'till this is complete. (Its a bit of a hack, I know.)
void handheld::handleTextCom(byte* comPtr) {

  textComs.handleTextCom((char*)&(comPtr[1]));  // Deal with passing on the text, getting reply sent.
  slaveReset();                                 // Not sending a reply here so we call the reset method to go back to listenting.    
}
