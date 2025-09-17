#include <GPS_NMEA2K.h>
#include <timestamp32bits.h>     // Gives the UNIX Epoch timestamp for PGN 0x1F805



byte     sid = 0;						// Gobal.. Err.. forgot what.


// Call this with your netObj and it'll add in all the GPS broadcasting handlers.
bool addGPSHandlers(netObj* inNetObj) {

	PGN0x1F801Handler*   ourPGN0x1F801;									// Pointers to our set.
	PGN0x1F802Handler*   ourPGN0x1F802;									// we have these three.
	PGN0x1F805Handler*   ourPGN0x1F805;									// To make up.
		
	ourPGN0x1F801 = new PGN0x1F801Handler(inNetObj);				// Create handelr for PGN 1F801
	ourPGN0x1F802 = new PGN0x1F802Handler(inNetObj);				// PGN 1F802
	ourPGN0x1F805 = new PGN0x1F805Handler(inNetObj);				// And 1F805
	if (ourPGN0x1F801 && ourPGN0x1F802 && ourPGN0x1F805) {		// If we got them all..
		inNetObj->addMsgHandler(ourPGN0x1F801);						// Add to netObj and let them deal with them.
		inNetObj->addMsgHandler(ourPGN0x1F802);						// If netObj gets deleted? It'll delete these as well.
		inNetObj->addMsgHandler(ourPGN0x1F805);						// 
		return true;															// Let 'em know it all worked out.
	}																				//
	if (ourPGN0x1F801) delete(ourPGN0x1F801);							// If we get here one or more failed to allocate.
	if (ourPGN0x1F802) delete(ourPGN0x1F802);							// So we clean up the mess.
	if (ourPGN0x1F805) delete(ourPGN0x1F805);							// Bt recycling them all.
	return false;																// Let 'em know it was a failure.
}


// Handy to have, currently don't have a good spot to store this. Hence we'll leave it
// here.
double degToRad(double inDeg) {

   return (inDeg * PI) / 180.0;
}



// ****************************************************
// PGN 0x1F801 - Position, Rapid Update
// ****************************************************


PGN0x1F801Handler::PGN0x1F801Handler(netObj* inNetObj)
   : msgHandler(inNetObj) { setSendInterval(100); }

   
PGN0x1F801Handler::~PGN0x1F801Handler(void) {  }


// Fill in to create messages.
void PGN0x1F801Handler::newMsg(void) {

     message   outMsg;

     outMsg.setPGN(0x1F801);
     outMsg.setPriority(2);
     outMsg.setSourceAddr(ourNetObj->getAddr());
     outMsg.setLongInData(0,ourGPS->latLon.getLatAsInt32());
     outMsg.setLongInData(4,ourGPS->latLon.getLonAsInt32());
      /*
     Serial.print(fixData.latLon.getLatQuadStr());Serial.print(" ");Serial.print(fixData.latLon.getLatDeg());Serial.print("º ");
     Serial.print(fixData.latLon.getLatMin(),3);Serial.println("'");
     Serial.print(fixData.latLon.getLonQuadStr());Serial.print(" ");Serial.print(fixData.latLon.getLonDeg());Serial.print("º ");
     Serial.print(fixData.latLon.getLonMin(),3);Serial.println("'");
     Serial.println();
    */
     //outMsg.showMessage();
     sendMsg(&outMsg);
}



// ****************************************************
// PGN 0x1F802: PGN 129026 - COG & SOG.
// ****************************************************


PGN0x1F802Handler::PGN0x1F802Handler(netObj* inNetObj)
   : msgHandler(inNetObj) { setSendInterval(100); }

   
PGN0x1F802Handler::~PGN0x1F802Handler(void) {  }


// Fill in to create messages.
void PGN0x1F802Handler::newMsg(void) {

      message   outMsg;
      double    COGDbl;
      int16_t   COGRad;
      double    SOGDbl;
      int16_t   SOGMpS;

      
      outMsg.setPGN(0x1F802);									// Set in our PGN.
      outMsg.setPriority(2);									// Priority.
      outMsg.setSourceAddr(ourNetObj->getAddr());		// Our address as source.
      outMsg.setDataByte(0,sid);								// serial ID. Basically a group ID for position data packets.
      outMsg.setDataByte(1,0);								// We're talking true course here.
      COGDbl = degToRad(ourGPS->trueCourse);				// We want true course, as a double.
      COGRad = round(COGDbl*1000);							// The integer version is that * 1000.
      outMsg.setIntInData(2,COGRad);						// And in it goes.
      SOGDbl = ourGPS->groundSpeedKilos * 0.277778;	// We have Kilometers per hour. We want meters per sec.
      SOGMpS = SOGDbl * 100;									// Multiply that by 100 gives our decimal value.
      outMsg.setIntInData(4,SOGMpS);						// pop it in.
      outMsg.setUIntInData(6,0xFFFF);						// Last two buytes are unused. Set to FF.
      //outMsg.showMessage();
      sendMsg(&outMsg);											// Off this version goes..
}


// ****************************************************
// PGN 0x1F805: PGN 129029 - GNSS Position Data
// ****************************************************


PGN0x1F805Handler::PGN0x1F805Handler(netObj* inNetObj)
   : msgHandler(inNetObj) { setSendInterval(1000); }

   
PGN0x1F805Handler::~PGN0x1F805Handler(void) {  }


// Fill in to create messages.           
void PGN0x1F805Handler::newMsg(void) {                 

   message           outMsg;
   int               numBytes;
   timestamp32bits   stamp;
   int               year;
   int               month;
   int               day;
   uint32_t          mins; 
   uint32_t          hours;
   uint32_t          days;
   uint32_t          secs;
   int64_t           ourInt64;
   byte              aByte;
   
   numBytes = 43;                                              // With no station data, it's just the 43 bytes.
   outMsg.setNumBytes(numBytes);                               // Try for the block of RAM.
   if (!outMsg.getNumBytes()) return;                          // If we didn't get them, we bail.
   outMsg.setPGN(0x1F805);                                     // Set in our PGN.
   outMsg.setPriority(3);                                      // Priority.
   outMsg.setSourceAddr(ourNetObj->getAddr());                 // Our address as source.
   outMsg.setDataByte(0,sid);                                  // serial ID. Basically a group ID for position data packets.
   year = ourGPS->year;														// Today's date comes from trasit data.
   month = ourGPS->month;                                 		// month
   day = ourGPS->day;                                     		// day
   mins = round(stamp.timestamp(year,month,day,0,0,0)/60.0);	// Here's the UNIX Epoch thing..
   hours = round(mins/60.0);												// Boild down to hours.
   days  = round(hours/24.0);												// Further reduced to days, what they want.
   outMsg.setUIntInData(1,days);											// Stuff in the data.
   secs = ourGPS->sec;														// Now they want seconds since midnight.
   secs = secs + ourGPS->min * 60;										// Add in the minutes as seconds.
   secs = secs + ourGPS->hours * 60 * 60;								// And the hours as seconds.
   outMsg.setULongInData(3,secs);										// Whatever! The docs are ambiguous on this point.
   ourInt64 = ourGPS->latLon.getLatAsInt64();                  // Grab the giant lat value.
   outMsg.setDLongInData(7,ourInt64);                          // Stuff it in.
   ourInt64 = ourGPS->latLon.getLonAsInt64();                  // Grab the giant lon value.
   outMsg.setDLongInData(15,ourInt64);                         // Stuff it in.
   ourInt64 = round(ourGPS->altitude * 1000000);               // Setup giant alt. data.
   outMsg.setDLongInData(23,ourInt64);                         // Stuff it in.
   aByte = (byte)ourGPS->qualVal;                              // This gives GPS type and quality all in one shot!
   outMsg.setDataByte(31,aByte);                               // In it goes.
   outMsg.setDataByte(32,0);                                   // We got no checking I can see. And the rest is reserved.
   outMsg.setDataByte(33,ourGPS->numSatellites);					// Stuff in the number of sattelites we found.
   outMsg.setIntInData(34,round(ourGPS->HDOP*100));				// We have HDOP.
   outMsg.setIntInData(36,round(ourGPS->PDOP*100));				// We have PDOP
   outMsg.setLongInData(38,round(ourGPS->GeoidalHeight*100));	// Got this too..
   outMsg.setDataByte(42,0);												// We don't seem to have any station ID's to give out.
	sendMsg(&outMsg);															// Off this mess goes.
}
