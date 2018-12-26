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
char* currentPN;

void setup() {

    FONAOnline = false;
    currentPN = NULL;

    pinMode(0,INPUT);                     // Adafruit says to do this. Otherwise it may read noise.
    pinMode(13, OUTPUT);
    ourComObj.begin(9600);                // For talking to the GUI.
    if (ourComObj.readErr()==NO_ERR) {    // Did the poor thing fire up?
      //digitalWrite(13, HIGH);
    }
    pinMode(FONA_RST, OUTPUT);            // Used for resetting the FONA.
    resetFONA();                          // Hit reaset, see if it'll come online.
    fona.setAudio(FONA_EXTAUDIO);         // Um.. Why is this here?
}


void loop() {
  byte  numBytes;
  byte* inBuff;
  
  idle();
  numBytes = ourComObj.haveBuff();
  if (numBytes>0) {
    inBuff = malloc(numBytes);
    if (inBuff) {
      if (ourComObj.readBuff(inBuff)) {
        switch(inBuff[0]) {
          case getStatus    : doStatus();               break;
          case setCurrentPN : doCurrentPH(&inBuff[1]);  break;  // Its a c string.
          case sendSNS      : doSendSNS(&inBuff[1]);    break;  // Its a c string.
          default           : break;
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
}


void doCurrentPH(char* inBuff) {

    byte    numBytes;
    uint8_t result;

    result = 0;                       // We'll pass a 0 for failure.
    if (currentPN) {                  // First, dump what we had.
      free(currentPN);                // Whoosh! There it goes.
    }
    numBytes = strlen(inBuff);        // Defined as a c string.
    currentPN = malloc(numBytes+1);   // Now, make room for the new number c string.  
    if (currentPN) {                  // We get the room?
        strcpy(currentPN,inBuff);     // Copy it over.
        result = 1;                   // We'll pass a 1 for success.
    }
    ourComObj.replyBuff((byte*)&result,sizeof(uint8_t)); 
}


void doSendSNS(char* inBuff) {

  uint8_t result;
  
  result = 0;
  if (fona.sendSMS(currentPN, inBuff)) {
    result = 1;
  }
  ourComObj.replyBuff((byte*)&result,sizeof(uint8_t));
}
