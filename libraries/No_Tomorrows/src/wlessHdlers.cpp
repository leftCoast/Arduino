#include <wlessHdlers.h>



// ************* rngBearingHdlr *************


rngBearingHdlr::rngBearingHdlr(netObj* inNetObj)
	: msgHandler(inNetObj) {

	rangeKn	= NAN;
	bearingT	= NAN;
	bearingM	= NAN;
}


rngBearingHdlr::~rngBearingHdlr(void) {  }

	
bool rngBearingHdlr::handleMsg(message* inMsg) {

	// FILL OUT AT HOME
	return false;
}


float rngBearingHdlr::getRangeKn(void) { return rangeKn; }


float rngBearingHdlr::getBearingT(void) { return bearingT; }


float rngBearingHdlr::getBearingM(void) { return bearingM; }
            
				


// ************* barometerHdlr *************


barometerHdlr::barometerHdlr(netObj* inNetObj)
   : msgHandler(inNetObj) {

   degF        = NAN;
   inHg        = NAN;
   deltaHg     = 0;
   inHgSmooth  = new runningAvg(6);
   inHg20Min   = new runningAvg(20);
   minuteTimer = new timeObj(60 * 1000);
}


barometerHdlr::~barometerHdlr(void) { 

	if (inHgSmooth) delete inHgSmooth;
	if (inHg20Min) delete inHg20Min;
	if (minuteTimer) delete minuteTimer;
}


bool barometerHdlr::handleMsg(message* inMsg) {

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
         gotPressure = true;									// Good enough!
      }                                               //
   } else if (inMsg->getPGN()==0x1FD06) {             // For this PGN version..
      rawPa16  = inMsg->getIntFromData(5);            // Grab the pressure data.
      if (!isBlank(rawPa16)) {                        // Make sure the data we want is actually there.
         Pa  = rawPa16 * 100;                         // Math
         inHg = inHgSmooth->addData(Pa*0.0002953);    // Math
         gotPressure = true;									// Good so far,
      }                                               // 
      rawTmp16 = inMsg->getIntFromData(3);            // This one also gives air temp, grab that.
      if (!isBlank(rawTmp16)) {                       // Make sure the data we want is actually there.
         kelvan = rawTmp16 / 100.0;							// Gives kelvan.
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

  
float barometerHdlr::getAirTemp(void) { return degF; }

float barometerHdlr::getInHg(void) { return inHg; }
