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
    rawSpeed = inMsg->getIntFromData(1);
    knots = speedMap.map(rawSpeed);
    Serial1.print("speed ");
    Serial1.println(knots);
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
    rawDepth = inMsg->getLongFromData(1);
    rawDepth = rawDepth / 100.0;    // Give meters.
    feet = rawDepth * 3.28084;      // Give feet
    Serial1.print("depth ");
    Serial1.println(feet/6.0);
    return true;
  }
  return false;
}

  
float waterDepthObj::getDepth(void) { return feet; }



// ************* waterTempObj *************


waterTempObj::waterTempObj(netObj* inNetObj)
   : msgHandler(inNetObj) { degF     = 0; }


waterTempObj::~waterTempObj(void) {  }


bool waterTempObj::handleMsg(message* inMsg) {

   uint16_t rawTemp;
   float    kelvan;
   
   if (inMsg->getPGN()==0x1FD08) {           // If we see the PGN we are looking for.
      rawTemp = inMsg->getUIntFromData(3);   // Grab the data.
      kelvan = rawTemp / 100.0;              // Gives kelvan.
      degF  = (kelvan * 1.8) - 459.67;       // Gives degF. uPdate the value.
      Serial1.print("watertemp ");
      Serial1.println(degF);
      return true;                           // Success we handled that one. return true.
   }                                         //
   return false;                             // Not ours, return false.
}

  
float waterTempObj::getTemp(void) { return degF; }


// ************* fluidLevelObj *************


 fluidLevelObj::fluidLevelObj(netObj* inNetObj) 
   : msgHandler(inNetObj) {
   
   fluidType = fuel;       // This is 0.
   level       = 0;        // Good as any for a default.
   setSendInterval(2500);  // Refresh the outgoing data every 2.5 seconds.
}

 
// Destructor. Nothing really to do here.
fluidLevelObj::~fluidLevelObj(void) {  }


// Forgot what kind of tank you got?
tankType fluidLevelObj::getTankType() { return fluidType; }


// There is a lookup llist of tank types that this value comes from.
void fluidLevelObj::setTankType(tankType inType) { fluidType = inType; }


// For you to read what value for level you set in. If we were to read the level
// from another's broadcast? It should be here.
float fluidLevelObj::getLevel(void) { return level; }


// Level is passed in as a percentage full.
void fluidLevelObj::setLevel(float inLevel) {
   
   level = inLevel;
   Serial1.print("fuel ");
   Serial1.println(level);
 }


// If you want to know the value you set in there. 
float fluidLevelObj::getCapacity(void) { return capacity; }


// I'm thinking this capacity is in gallons. It'll be converted when broadcasted.
void fluidLevelObj::setCapacity(float inCapacity) { capacity = inCapacity; }


// We can't currently disply the fluid level others send out.
bool fluidLevelObj::handleMsg(message* inMsg) { return false; }
  

// Every set amount of time a new set of values will be sent out.
// This is where that happens.
void fluidLevelObj::newMsg(void) {
      
   message  outMsg;
   int32_t  calcPercent;
   byte     aByte;
   uint32_t decaLiters;
   
   outMsg.setPGN(0x1F211);                            // PGN we will be broadcasting.
   outMsg.setPriority(6);                             // I read 6 is the value in this case.
   outMsg.setSourceAddr(ourNetObj->getAddr());        // Our current return address.
   aByte = 0;                                         // Tanks instance is zero in this example.
   aByte = aByte<<4;                                  // If not zero, this'll shift it over.
   aByte = aByte | ((byte)fluidType & 0b00001111);    // Fuel, diesil? is 0x00. 
   outMsg.setDataByte(0,aByte);                       // Anyway instance and fluid type together in this byte goes in.
   calcPercent = round(level*250);                    // Two byte value gets level percentage * 250. 
   outMsg.setIntInData(1,calcPercent);                // Pack the result as signed number into bytes 1 & 2.
   decaLiters = capacity * 37.8541;                   // Now, dealing with capacity's units.
   outMsg.setLongInData(3,decaLiters);                // In goes the unsigned long pack. bytes 3,4,5,6
   outMsg.setDataByte(7,0xFF);                        // Reserved, so..
   sendMsg(&outMsg);                                  // Zoom! Off it goes!
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
   if (success) {
      Serial1.print("barometer ");
      Serial1.println(inHg);
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
