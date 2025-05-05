#include "NMEAHandlers.h"

//PGN 0x1F801 - Position, Rapid Update

PGN0x1F801Handler::PGN0x1F801Handler(netObj* inNetObj)
   : msgHandler(inNetObj) { setSendInterval(100); }

   
PGN0x1F801Handler::~PGN0x1F801Handler(void) {  }

GPVTG    trackMadeGood;  
GPGGA    fixData; 
GPGSA    activeSatellites; 
GPGSV    SatellitesInView; 
GPRMC    minTransData;


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
