#include <handlers.h>
#include <strTools.h>


// ************* waterSpeedObj *************


waterSpeedObj::waterSpeedObj(netObj* inNetObj)
   : msgHandler(inNetObj) {
      
  knots     = 0;
  speedMap.setValues(0,1023,0,(1023*1.943844)*0.01);
}


waterSpeedObj::~waterSpeedObj(void) {  }


bool waterSpeedObj::handleMsg(message* inMsg) {

  unsigned int rawSpeed;
  
  if (inMsg->getPGN()==0x1F503) {
    rawSpeed = inMsg->getIntFromData(1);
    knots = speedMap.map(rawSpeed);
    return true;
  }
  return false;
}

  
float waterSpeedObj::getSpeed(void) { return knots; }

 

// ************* waterDepthObj *************


waterDepthObj::waterDepthObj(netObj* inNetObj)
   : msgHandler(inNetObj) { feet      = 0; }


waterDepthObj::~waterDepthObj(void) {  }


bool waterDepthObj::handleMsg(message* inMsg) {

  unsigned int rawDepth;
  
  if (inMsg->getPGN()==0x1F50B) {
    rawDepth = inMsg->getLongFromData(1);
    rawDepth = rawDepth / 100.0;    // Give meters.
    feet = rawDepth * 3.28084;      // Give feet
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
   //setSendInterval(2500);  // Refresh the outgoing data every 2.5 seconds.
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
void fluidLevelObj::setLevel(float inLevel) { level = inLevel; }


// If you want to know the value you set in there. 
float fluidLevelObj::getCapacity(void) { return capacity; }


// I'm thinking this capacity is in gallons. It'll be converted when broadcasted.
void fluidLevelObj::setCapacity(float inCapacity) { capacity = inCapacity; }


// We can't currently disply the fluid level others send out.
bool fluidLevelObj::handleMsg(message* inMsg) {

   byte        firstByte;
   int			tankVal;
   byte        instance;
   uint16_t    rawLevel;
   //uint32_t    rawCap;
   //float       liters;
   
   if (inMsg->getPGN()==0x1F211) {
      firstByte = inMsg->getDataByte(0);
      instance = firstByte;
      instance = 0x0F & instance;
      tankVal = firstByte;
      fluidType = (tankType)(tankVal >> 4);
      rawLevel = inMsg->getUIntFromData(1);
      level = rawLevel/250.0;
      //rawCap = inMsg->getULongFromData(3);
      //liters = rawCap/10.0;
      /*
      Serial.print("instance : ");
      Serial.println(instance);
      Serial.print("fluid Type : ");
      Serial.println(fluidType);
      
      Serial.print("level : ");
      Serial.print(level);
      Serial.println(" %");
      */
      return true;
   }
   return false;
}
  

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



// ************* barometerObj *************


barometerObj::barometerObj(netObj* inNetObj)
   : msgHandler(inNetObj) {

   degF        = 0;
   inHg        = 0;
   deltaHg     = 0;
   inHgSmooth  = new runningAvg(6);
   inHg20Min   = new runningAvg(20);
   minuteTimer = new timeObj(60 * 1000);
}


barometerObj::~barometerObj(void) { if (inHgSmooth) delete inHgSmooth; }


bool barometerObj::handleMsg(message* inMsg) {

   uint32_t rawPa32;
   uint16_t rawPa16;
   uint16_t rawTmp16;
   float    Pa;
   bool     gotPressure;
   bool     gotTemp;
   float    kelvan;
   
   gotPressure = false;
   gotTemp = false;
   if (inMsg->getPGN()==0x1FD0A) {                    // For this PGN version..
      rawPa32 = inMsg->getULongFromData(3);           // Grab the data.
      if (!isBlank(rawPa32)) {                        // Make sure the data we want is actually there.
         Pa  = rawPa32 * 0.1;                         // Math
         inHg = inHgSmooth->addData(Pa*0.0002953);    // Math
         gotPressure = true;                              // Good enough!
      }                                               //
   } else if (inMsg->getPGN()==0x1FD06) {             // For this PGN version..
      rawPa16  = inMsg->getIntFromData(5);            // Grab the pressure data.
      if (!isBlank(rawPa16)) {                        // Make sure the data we want is actually there.
         Pa  = rawPa16 * 100;                         // Math
         inHg = inHgSmooth->addData(Pa*0.0002953);    // Math
         gotPressure = true;                              // Good so far,
      }                                               // 
      rawTmp16 = inMsg->getIntFromData(3);            // This one also gives air temp, grab that.
      if (!isBlank(rawTmp16)) {                       // Make sure the data we want is actually there.
         kelvan = rawTmp16 / 100.0;                    // Gives kelvan.
         degF  = (kelvan * 1.8) - 459.67;             // Gives degF. uPdate the value.
         gotTemp = true;                              // Success we handled that one.
      }                                               //
   } else if (inMsg->getPGN()==0x1FD07) {             // For this PGN version..
      rawPa16  = inMsg->getIntFromData(6);            // Grab the data.
      if (!isBlank(rawPa16)) {                        // Make sure the data we want is actually there.
         Pa  = rawPa16 * 100;                         // More math
         inHg = inHgSmooth->addData(Pa*0.0002953);    // And more math
         gotPressure = true;                          // There, have presssure.
      }                                               //
      rawTmp16 = inMsg->getIntFromData(2);            // This one also goves air temp, so we grab that.
      if (!isBlank(rawTmp16)) {                       // Make sure the data we want is actually there.
         kelvan = rawTmp16 / 100.0;                   // Gives kelvan.
         degF  = (kelvan * 1.8) - 459.67;             // Gives degF. uPdate the value.
         gotTemp = true;                              // Success we handled that one.
      }                                               //
   }                                                  //
   if (gotPressure && minuteTimer->ding()) {          // If we got a pressure reading AND the timer went off..
      inHg20Min->addData(inHg);                       // Stuff the smoothed reading into the 20 minute list.
      if (inHg20Min->getNumValues()==20) {            // If we been at this for at least twenty minutes.
         deltaHg = inHg20Min->getEndpointDelta();     // We calcualte the rate of change over 20 minutes.
         deltaHg = deltaHg * 3.0;                     // The deal is 0.06 inHg per hour, for at least 20 minutes, to call it rapid.
      } else {                                        // Else not full yet..
         deltaHg = NAN;                               // Then we call it a NAN.
      }                                               // 
      minuteTimer->stepTime();                        // Restart the timer for the next minute reading.
   }                                                  //
   return gotTemp || gotPressure;                     // We'll call either a sccess.
}

  
float barometerObj::getAirTemp(void) { return degF; }

float barometerObj::getInHg(void) { return inHg; }



// ************* engParam *************


engParam::engParam(netObj* inNetObj)
	: msgHandler(inNetObj) {
	
	encodeRPM	= false;		// Docs. say RPM should be radians/sec * 4. Reality says no.
	engInst		= 0;
	RPM			= 0;
  	boostPSI		= 0;
	tiltPerc		= 0;
	//setSendInterval(100);  // Refresh the outgoing data every 100 ms. 
}
	
	
engParam::~engParam(void) {  }


void engParam::setEncodeRPM(bool trueFalse) { encodeRPM = trueFalse; }

        
bool engParam::handleMsg(message* inMsg) {
	
	uint16_t	rawRPM;
	uint16_t	rawBoost;
	
	if (inMsg->getPGN()== 0x1F200) {					// If it's our PGN..
		if (inMsg->getDataByte(0)==engInst) {		// If it's OUR engine..
			rawRPM = inMsg->getUIntFromData(1);		// Grab the RPM info.
			if (encodeRPM) {
				RPM = (rawRPM * RADPS_RPM)/4.0;		// Do the calculation.
			} else {
				RPM = rawRPM/4;							// Or not..
			}
			RPM = (rawRPM * RADPS_RPM)/4.0;			// Do the calculation.
			rawBoost = inMsg->getUIntFromData(3);	// Grab the boost info.
			rawBoost = rawBoost * 100;
			boostPSI = rawBoost * PAS_PSI;
			tiltPerc = inMsg->getDataByte(5);			// Grab tilt data.
			return true;		
		}
	}
	return false;
}


void engParam::newMsg(void) {

	message	outMsg;
	int		NMEA_rps;
	int		NMEA_boost;
	
	if (encodeRPM) {											
		NMEA_rps = round(RPM*RPM_RADPS*4);				// Docs say radians/sec.
	} else {
		NMEA_rps = round(RPM*4);							// In the field its RPM.
	}
	NMEA_boost = round((boostPSI*PSI_PAS)/100.0);
	outMsg.setPGN(0x1F200);									// PGN we will be broadcasting.
   outMsg.setPriority(2);									// I read 2 is the value in this case.
   outMsg.setSourceAddr(ourNetObj->getAddr());		// Our current return address.
   outMsg.setDataByte(0,engInst);						// Our engine.
   outMsg.setUIntInData(1,NMEA_rps);					// RPM converted. Or not.
   outMsg.setUIntInData(3,NMEA_boost);					// Bost value converted.
   outMsg.setDataByte(5,tiltPerc);						// Tilt percent..
   outMsg.setUIntInData(6,0xFFFF);						// Reserved, so..
   sendMsg(&outMsg);											// Zoom! Off it goes!
}


void engParam::setEngInst(int inst) {

	if (inst>252) {
		engInst = 252;
	} else if (inst<0) {
		engInst = 0;
	} else {
		engInst = inst;
	}
}


float engParam::getEngInst(void) { return engInst; }


void engParam::setRPM(float inRPM) { RPM = inRPM; }
	

float engParam::getRPM(void) { return RPM; }


// Check bounds when sending this one.
void engParam::setBoost(float boost) { boostPSI = boost; }
	
		
float engParam::getBoost(void) { return boostPSI; }


void engParam::setTilt(float tilt) {

	if (tilt>100) {
		tiltPerc = 100;
	} else if (tilt<0) {
		tiltPerc = 0;
	} else {
		tiltPerc = round(tilt);
	}
}


float engParam::getTilt(void) { return tiltPerc; }



// ************* engParamII *************

#define ENG_PARAMII_BYTES	26

engParamII::engParamII(netObj* inNetObj)
	: msgHandler(inNetObj) {
	
	engInst	= 0;
	setSendInterval(500);  // Refresh the outgoing data every 100 ms. 
}
	
	
engParamII::~engParamII(void) {  }

        
bool engParamII::handleMsg(message* inMsg) {
	
	uint16_t	status1;
	
	if (inMsg->getPGN()== 0x1F201) {					// If it's our PGN..
		if (inMsg->getDataByte(0)==engInst) {		// If it's OUR engine..
			status1 = inMsg->getUIntFromData(20);	// Grab the the info.
			ourAlarms.overTemp = bitRead(status1,1);
			ourAlarms.lowOilPSI = bitRead(status1,2);
			return true;		
		}
	}
	return false;
}


void engParamII::newMsg(void) {

	message	outMsg;
	uint16_t	status1;
	
	outMsg.setNumBytes(ENG_PARAMII_BYTES);				// It's bigger than usual.
	if (outMsg.getNumBytes()==ENG_PARAMII_BYTES) {	// If we got them..
		for (int i=0;i<ENG_PARAMII_BYTES;i++) {		// Stomp FF into all the data to flag as unused.
			outMsg.setDataByte(i,0xFF);					// Stomp stomp stomp!
		}															// Because we're not using most of it.
		outMsg.setPGN(0x1F201);								// PGN we will be broadcasting.
		outMsg.setPriority(2);								// I read 2 is the value in this case.
		outMsg.setSourceAddr(ourNetObj->getAddr());	// Our current return address.
		outMsg.setDataByte(0,engInst);					// Our engine.
		status1 = 0;											// Clear this one.
		if (ourAlarms.overTemp) {							// Grab temp value.
			bitSet(status1,1);								// Set bit if alarm is active.
		}															// Take a breath!
		if (ourAlarms.lowOilPSI) {							// Grab temp value.
			bitSet(status1,2);								// Set bit if alarm is active.
		}															//
		outMsg.setUIntInData(20,status1);				// 
		sendMsg(&outMsg);										// Zoom! Off it goes!
	}
}


void engParamII::setEngInst(int inst) {

	if (inst>252) {
		engInst = 252;
	} else if (inst<0) {
		engInst = 0;
	} else {
		engInst = inst;
	}
}


float engParamII::getEngInst(void) { return engInst; }


void engParamII::setAlarms(alarms* inAlarms) { ourAlarms = *inAlarms; }
	

alarms engParamII::getAlarms(void) { return ourAlarms; }



// ****************************************************
// PGN 0x1F904: PGN 129029 - Navigation Data
// ****************************************************


PGN0x1F904Handler::PGN0x1F904Handler(netObj* inNetObj)
   : msgHandler(inNetObj) { setSendInterval(1000); }

   
PGN0x1F904Handler::~PGN0x1F904Handler(void) {  }


// Fill in to create messages.           
void PGN0x1F904Handler::newMsg(void) {                 

   message	outMsg;
   int		numBytes;
   byte     sid;
   float		meters;
   int32_t	meters32;
   byte		aByte;
   uint16_t	bearingU16;
   int16_t	knotsMadeGood;

   numBytes = 34;													// Doc. says 34 bytes.
   outMsg.setNumBytes(numBytes);								// Try for the block of RAM.
   if (!outMsg.getNumBytes()) return;						// If we didn't get them, we bail.
   outMsg.setPGN(0x1F904);										// Set in our PGN.
   outMsg.setPriority(3);										// Priority.
   outMsg.setSourceAddr(ourNetObj->getAddr());			// Our address as source.
  	sid = 0;															// Gobal.. Err.. Forgot what.
   outMsg.setDataByte(0,sid);									// serial ID. Serial ID for data packets?
   meters = distToWP * 1852;									// They want meters?! Nm -> meters;
   meters32 = round(meters * 100);							// Integer x 100.
   outMsg.setLongInData(1,meters32);						// And we stuff it in.
   aByte = 0;														// Clear out the byte
   if (magnetic) {												// Magnetic means 1 (lower two bits).
   	aByte = 1;													// Good enough.
   }																	//
   if (perpCrossed) {											// Perpendicular crossed?
   	aByte = aByte | 0b00000100;							// Next two bits set to value one.
   }																	//
   if (inMinRange) {												// Close enough to the mark to say we're there?
   	aByte = aByte | 0b00010000;							// Next two bits set to value one.
   }																	//
   aByte = aByte | 0b01000000;								// Next two bits? Always using great circle.
   outMsg.setDataByte(5,aByte);								// Stuff in the byte.
   outMsg.setLongInData(6,0);									// No idea what they want here.
   outMsg.setUIntInData(10,0);								// Here either.
   bearingU16 = round(bearingFromStart * 10000);		// Calculate the int bearing.
   outMsg.setUIntInData(12,bearingU16);					// Bearing from start.
   bearingU16 = round(bearingFromFix * 10000);			// Calculate the int bearing.
   outMsg.setUIntInData(14,bearingU16);					// Bearing from fix.
   outMsg.setULongInData(16,startWPNum);					// Set in waypoint number. If we have one..
   outMsg.setULongInData(20,endWPNum);						// Set the endpoint waypoint number. If we have one.
   outMsg.setLongInData(24,endPos.getLatAsInt32());	// Set Destination lat.
   outMsg.setLongInData(28,endPos.getLonAsInt32());	// Set Destination lon.
	knotsMadeGood = round(knMadeGood * 100);				// Calculate the int version.
	outMsg.setIntInData(32,knotsMadeGood);					// Stuff it into the message.
	sendMsg(&outMsg);												// And off it goes!
}



// ************* LC_ChatObj *************
/*
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

*/
