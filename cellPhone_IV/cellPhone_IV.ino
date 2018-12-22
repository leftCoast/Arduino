#include <idlers.h>
#include <lists.h>
#include <timeObj.h>
#include <quickCom.h>
#include <cellCommon.h>

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

qCSlave   ourComObj;

bool  FONAOnline;


void setup() {

    Serial.begin(9600);
    pinMode(FONA_RST, OUTPUT);        // Used for resetting the FONA.
    ourComObj.begin(9600);            // For talking to the GUI.
    resetFONA();                      // Hit reaset, see if it'll come online.
    fona.setAudio(FONA_EXTAUDIO);     // Um.. Why is this here?
}


void loop() {
  byte  numBytes;
  byte* inBuff;
  
  idle();
  numBytes = ourComObj.haveBuff();
  if (numBytes>0) {
    Serial.print("got byte count : ");Serial.println(numBytes);
    inBuff = malloc(numBytes);
    if (inBuff) {
      if (ourComObj.readBuff(inBuff)) {
        switch(inBuff[0]) {
          case getStatus  : doStatus(); break;
          default         : break;
        } 
      }
      free(inBuff);
      inBuff = NULL;
    }
  } 
}


// *************************************************
// ******************* handlers ********************
// *************************************************

void resetFONA(void) {

  FONAOnline = false;
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  digitalWrite(FONA_RST, LOW);
  delay(10);
  digitalWrite(FONA_RST, HIGH);
  delay(100);
  fonaSS.begin(4800);                 // For talking to the FONA.
  FONAOnline = fona.begin(fonaSS);   // Able to fire up the FONA.
}


void doStatus(void) {

  cellStatus  newStat;

  newStat.FONAOnline = FONAOnline;
  fona.getBattVoltage(&newStat.batteryVolts);
  fona.getBattPercent(&newStat.batteryPercent);
  newStat.RSSI = fona.getRSSI();
  newStat.networkStat = (networkStatus)fona.getNetworkStatus();
  newStat.volume = fona.getVolume();
  newStat.callStat = (callStatus)fona.getCallStatus();
  newStat.numSMSs = fona.getNumSMS();
  fona.getTime(newStat.networkTime,23);

  ourComObj.replyBuff((byte*)&newStat,sizeof(cellStatus));
  Serial.print("cellStatus : ");Serial.println(sizeof(cellStatus));
}


  
