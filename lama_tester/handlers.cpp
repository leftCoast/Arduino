#include "handlers.h"
#include <strTools.h>


// ************* waterSpeedObj *************


waterSpeedObj::waterSpeedObj(netObj* inNetObj)
   : msgHandler(inNetObj) {

  knots   = 0;
  speedMap.setValues(0,1023,0,(1023*1.943844)*0.01);
}


waterSpeedObj::~waterSpeedObj(void) {  }


bool waterSpeedObj::handleMsg(message* inMsg) {

  unsigned int rawSpeed;
  
  if (inMsg->getPGN()==0x1F503) {
    rawSpeed = (unsigned int) pack16(inMsg->getDataByte(2),inMsg->getDataByte(1));
    knots = speedMap.map(rawSpeed);
    return true;
  }
  return false;
}

  
float waterSpeedObj::getSpeed(void) { return knots; }



// ************* waterDepthObj *************


waterDepthObj::waterDepthObj(netObj* inNetObj)
   : msgHandler(inNetObj) {

  feet   = 0;
}


waterDepthObj::~waterDepthObj(void) {  }


bool waterDepthObj::handleMsg(message* inMsg) {

  unsigned int rawDepth;
  if (inMsg->getPGN()==0x1F50B) {
    rawDepth = (unsigned int) pack32(inMsg->getDataByte(4),inMsg->getDataByte(3),inMsg->getDataByte(2),inMsg->getDataByte(1));
    rawDepth = rawDepth / 100.0;    // Give meters.
    feet = rawDepth * 3.28084;      // Give feet
    return true;
  }
  return false;
}

  
float waterDepthObj::getDepth(void) { return feet; }



// ************* waterTempObj *************


waterTempObj::waterTempObj(netObj* inNetObj)
   : msgHandler(inNetObj) {

   degF     = 0;
}


waterTempObj::~waterTempObj(void) {  }


bool waterTempObj::handleMsg(message* inMsg) {

   unsigned int rawTemp;

  if (inMsg->getPGN()==0x1FD08) {
    rawTemp  = (unsigned int) pack16(inMsg->getDataByte(4),inMsg->getDataByte(3));
    degF  = rawTemp / 100.0;         // Give kelvan.
    degF  = (degF * 1.8) - 459.67;   // Give degF.
    return true;
   }
   return false;
}

  
float waterTempObj::getTemp(void) { return degF; }


// ************* fluidLevelObj *************


 fluidLevelObj::fluidLevelObj(netObj* inNetObj) 
   : msgHandler(inNetObj) {
   
  fluidType = fuel;  // This is 0.
   level       = 0;
   setSendInterval(2500);
}
 

fluidLevelObj::~fluidLevelObj(void) {  }

tankType fluidLevelObj::getTankType() { return fluidType; }

void fluidLevelObj::setTankType(tankType inType) { fluidType = inType; }

float fluidLevelObj::getLevel(void) { return level; }

void fluidLevelObj::setLevel(float inLevel) { level = inLevel; }

float fluidLevelObj::getCapacity(void) { return capacity; }

void fluidLevelObj::setCapacity(float inCapacity) { capacity = inCapacity; }


// Using this guy to debug/see what's going on with the fuel sensor. Looks like all it gives me is zeros.
bool fluidLevelObj::handleMsg(message* inMsg) {
  /*
  if (inMsg->getSourceAddr()==187) {
    inMsg->showMessage();
    Serial.println();
    return true;
  }
  */
  return false;
}
  
  
void fluidLevelObj::newMsg(void) {
   
   int16_t  tempInt;
   int32_t  tempLong;
  message outMsg;
  byte    aByte;
  
  outMsg.setPGN(0x1F211);
   outMsg.setPriority(6);
   outMsg.setSourceAddr(ourNetObj->getAddr());
   
   aByte = 0;                             // instance is zero in this example.
   aByte = aByte<<4;                        // Not zero, this'll shift it over.
   aByte = aByte | ((byte)fluidType & 0b00001111); 
   outMsg.setDataByte(0,aByte);
   
   tempInt = round(level*250);
   aByte = tempInt & 0x00FF;
   outMsg.setDataByte(1,aByte);
   
   aByte = (tempInt & 0xFF00)>>8;
   outMsg.setDataByte(2,aByte);
   
   tempLong = round(capacity * 10);
   aByte = tempLong & 0x000000FF;
   outMsg.setDataByte(3,aByte);
   
   aByte = (tempLong & 0x0000FF00)>>8;
   outMsg.setDataByte(4,aByte);
   
   aByte = (tempLong & 0x00FF0000)>>16;
   outMsg.setDataByte(5,aByte);
   
   aByte = (tempLong & 0xFF000000)>>24;
   outMsg.setDataByte(6,aByte);
   
   aByte = 0xFF; 
   outMsg.setDataByte(7,aByte);                           // Reserved, so..
   
    sendMsg(&outMsg);
}


// ************* airTempBarometer *************


airTempBarometer::airTempBarometer(netObj* inNetObj)
   : msgHandler(inNetObj) {

   degF = 0;
   inHg = 0;
   inHgSmooth = new runningAvg(6);
}


airTempBarometer::~airTempBarometer(void) { if (inHgSmooth) delete inHgSmooth; }


bool airTempBarometer::handleMsg(message* inMsg) {

   uint32_t rawPa32;
   uint16_t rawPa16;
   float    Pa;
  bool    success;
  
  success = false;
  if (inMsg->getPGN()==0x1FD0A) {
    rawPa32  = pack32(inMsg->getDataByte(6),inMsg->getDataByte(5),inMsg->getDataByte(4),inMsg->getDataByte(3));
    if (!isBlank(rawPa32)) {                // Make sure the data we want is actually there.
      Pa  = rawPa32 * 0.1;
      inHg = inHgSmooth->addData(Pa*0.0002953);
      success = true;
    }
   } else if (inMsg->getPGN()==0x1FD06) {
    rawPa16  = pack16(inMsg->getDataByte(6),inMsg->getDataByte(5));
    if (!isBlank(rawPa16)) {                // Make sure the data we want is actually there.
      Pa  = rawPa16 * 100;
      inHg = inHgSmooth->addData(Pa*0.0002953);
      success = true;
    }
   } else if (inMsg->getPGN()==0x1FD07) {
    rawPa16  =  pack16(inMsg->getDataByte(7),inMsg->getDataByte(6));
    if (!isBlank(rawPa16)) {                // Make sure the data we want is actually there.
      Pa  = rawPa16 * 100;
      inHg = inHgSmooth->addData(Pa*0.0002953);
      success = true;
    }
   }
   return success;
}

  
float airTempBarometer::getAirTemp(void) { return degF; }

float airTempBarometer::getInHg(void) { return inHg; }



// ************* LC_ChatObj *************

LC_ChatObj::LC_ChatObj(netObj* inNetObj)
   : msgHandler(inNetObj) {
   
   outStr = NULL;         // If using LC stuff, ALL pointers need to start at NULL.
   setSendInterval(250);  // Lets at least check the string for an update this often.
}


LC_ChatObj::~LC_ChatObj(void) { freeStr(&outStr); }


bool LC_ChatObj::handleMsg(message* inMsg) {

  int   numBytes;
  bool  success;
  
  success = false;
  if (inMsg) {                                        // Sanity, not NULL.
    if (inMsg->getPDUf()==0x00E1) {                   // If it's our stolen peer to peer PDUf.
      if (inMsg->getPDUs()==ourNetObj->getAddr()) {   // And it's adressed to us!
        numBytes = inMsg->getNumBytes();              // Grab the number of data bytes.
        for(int i=0;i<numBytes;i++) {                 // For each byte..
          Serial.print((char)inMsg->getDataByte(i));        // Print it out.
        }                                             //
        Serial.println();                             // Line feed to make it pretty.
        success = true;                               // We are a complete success!
      }
    }
  }
  return success;
}
 

void LC_ChatObj::setOutStr(char* inStr) { heapStr(&outStr,inStr); }


void LC_ChatObj::clearOutStr(void) { freeStr(&outStr); }


void LC_ChatObj::newMsg(void) {
  
  message outMsg;
  int     numChars;

  if (outStr) {                                     // Non NULL outstring means we have one to send.
    Serial.print("Got string : [");
    Serial.print(outStr);
    Serial.println("]");
    numChars = strlen(outStr);                      // Count the chars we need to send out.
    outMsg.setNumBytes(numChars);                   // Grab the RAM for the meesage to hold them.
    if (outMsg.getNumBytes()==numChars) {           // If we got the RAM to store them..
      outMsg.setPGN(0x00E100);                      // Start setting up our message. Stolen PGN goes in.
      outMsg.setPDUs(OTHER_ADDR);                   // This PGN is a peer to peer, so typcially their addr goes here.
      outMsg.setPriority(DEF_PRIORITY);             // Set priority.                       
      outMsg.setSourceAddr(ourNetObj->getAddr());   // Our address..
      for (int i=0;i<numChars;i++) {                // For each char in our string..
        outMsg.setDataByte(i,outStr[i]);            // Stuff it into the data section of the message.
      }                                             //
      //outMsg.showMessage();
      sendMsg(&outMsg);                             // Send in this message to be transmitted over the wire.
      clearOutStr();                                // Clear the string as a flag. (Recycles it and makes it a NULL string.)
    }
  }
}
