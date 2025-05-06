#include "NMEAHandlers.h"

// The handlers from the GPS. (Our data source)
GPVTG    trackMadeGood;  
GPGGA    fixData; 
GPGSA    activeSatellites; 
GPGSV    SatellitesInView; 
GPRMC    minTransData;
byte     sid = 0;

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
     outMsg.setLongInData(0,fixData.latLon.getLatAsInt32());
     outMsg.setLongInData(4,fixData.latLon.getLonAsInt32());
     outMsg.showMessage();
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

      
      outMsg.setPGN(0x1F802);                                   // Set in our PGN.
      outMsg.setPriority(2);                                    // Priority.
      outMsg.setSourceAddr(ourNetObj->getAddr());               // Our address as source.
      outMsg.setDataByte(0,sid);                                // serial ID. Basically a group ID for position data packets.
      outMsg.setDataByte(1,0);                                  // We're talking true course here.
      COGDbl = degToRad(trackMadeGood.trueCourse);              // We want true course, as a double.
      COGRad = round(COGDbl*1000);                              // The integer version is that * 1000.
      outMsg.setIntInData(2,COGRad);                            // And in it goes.
      SOGDbl = trackMadeGood.groundSpeedKilosPH * 0.277778;     // We have Kilometers per hour. We want meters per sec.
      SOGMpS = SOGDbl * 100;                                    // Multiply that by 100 gives our decimal value.
      outMsg.setIntInData(4,SOGMpS);                            // pop it in.
      outMsg.setUIntInData(6,0xFFFF);                           // Last two buytes are unused. Set to FF.
      outMsg.showMessage();
      sendMsg(&outMsg);                                         // Off this version goes..
}
