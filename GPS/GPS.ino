
#include <SAE_J1939.h>
#include <llama2000.h>

#include <GPxxx.h>
#include <GPSHandlers.h>
#include "NMEAHandlers.h"

#define LC_MANF      35    // Left coast's manufacturrere # (I made it up.) As in J/35. My J/35. What all this was designed for.
#define LLAMA_CS     10    // llama board chip select pin.
#define LLAMA_RST    6     // llama board reset pin.
#define LLAMA_INT    2     // llama board inturrupt pin.
#define LLAMA_ADDR   46    // llama board default starting address.

llama2000*           llamaBrd;   // The class that inherits netObj, adding our attachment to hardware.
PGN0x1F801Handler*   ourPGN0x1F801;
PGN0x1F802Handler*   ourPGN0x1F802;
PGN0x1F805Handler*   ourPGN0x1F805;
GPSInStr ourGPS;

void setup() {

   Serial.begin(9600);
   Serial1.begin(9600);
   delay(100);
   llamaBrd = new llama2000(LLAMA_RST,LLAMA_INT);
   if (llamaBrd) {
      
      ourPGN0x1F801 = new PGN0x1F801Handler(llamaBrd);
      llamaBrd->addMsgHandler(ourPGN0x1F801);
      ourPGN0x1F802 = new PGN0x1F802Handler(llamaBrd);
      llamaBrd->addMsgHandler(ourPGN0x1F802);
      ourPGN0x1F805 = new PGN0x1F805Handler(llamaBrd);
      llamaBrd->addMsgHandler(ourPGN0x1F805);
   
      llamaBrd->setID(6387);                    // Device ID. We make these up. You get 21 bits.
      llamaBrd->setManufCode(LC_MANF);          // This would be assigned to you by NMEA people.
      llamaBrd->setECUInst(0);                  // First netObj (Electronic control unit.)
      llamaBrd->setFunctInst(0);                // First GPS.
      llamaBrd->setFunction(DEV_FUNC_GNSS);     // GPS.
      llamaBrd->setVehSys(DEV_CLASS_NAV);       // We are an instrument.
      llamaBrd->setSystemInst(0);               // We are the first of our device class.
      llamaBrd->setIndGroup(Marine);            // What kind of machine are we ridin' on?
      
      if (llamaBrd->begin(LLAMA_ADDR,arbitraryConfig,LLAMA_CS)) {
         ourGPS.addHandler(&trackMadeGood);
         ourGPS.addHandler(&fixData);
         ourGPS.addHandler(&activeSatellites);
         ourGPS.addHandler(&SatellitesInView);
         ourGPS.addHandler(&minTransData);
      } else {
         Serial.println("llama failed to fire up.");
         Serial.println("Halting process");
         while(1);
      }
   } else {
      Serial.println("Can't create the llamaBrd netObj.");
      Serial.println("Halting process");
      while(1);
   }
}


void loop() {
   
   idle();
}
