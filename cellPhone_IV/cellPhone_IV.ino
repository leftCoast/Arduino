#include <idlers.h>
#include <lists.h>
#include <quickCom.h>
#include "cellCommon.h"

#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4
#define FONA_RI  7


void blink(int numBlinks) {

  for (int i=0;i<numBlinks;i++) {
    digitalWrite(13, HIGH);
    delay(75);
    digitalWrite(13, LOW);
    delay(150);
  }
  delay(250);
}


SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

qCSlave     ourComObj;
bool        FONAOnline;
cellStatus  curStat;
char        inBuff[30];

void setup() {

    
    FONAOnline = false;
  
    pinMode(0,INPUT);                     // Adafruit says to do this. Otherwise it may read noise.
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    ourComObj.begin(9600);                // For talking to the GUI.
    if (ourComObj.readErr()==NO_ERR) {    // Did the poor thing fire up?
      digitalWrite(13, LOW);
    } else {
      digitalWrite(13, HIGH);
    }
    pinMode(FONA_RST, OUTPUT);            // Used for resetting the FONA.
    resetFONA();                          // Hit reset, see if it'll come online.
}


void loop() {
  byte  numBytes;
  byte* inBuff;
  
  idle();
  numBytes = ourComObj.haveBuff();
  if (numBytes>0) {
    digitalWrite(13, HIGH);
    if (numBytes>30)  {
      blink(25);
    } else {
      if (ourComObj.readBuff(inBuff)) {   // Its a c string.
        switch(inBuff[0]) {
          case getStatus    : doStatus();               break;
          case makeCall     : doCall(&inBuff[1]);       break;  
          case hangUp       : doHangUp( );              break;
          case sendSNS      : doSendSNS(&inBuff[1]);    break;
          case pickUp       : doPickUp();               break;
          default           : break;
        } 
      }
    }
  }
  digitalWrite(13, LOW); 
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
  fonaSS.begin(4800);                   // For talking to the FONA.
  FONAOnline = fona.begin(fonaSS);      // Able to fire up the FONA.
  fona.setAudio(FONA_EXTAUDIO);         // Um.. Why is this here?
  fona.setVolume(40);
  fona.enableNetworkTimeSync(true);     // See if it works..

/*
For those who wonder : You can add a call fona
setAudio(FONA_EXTAUDIO) to your initialization code or issue
direct command 'AT+CHFA=1' towards SIM800 module.
In my case this module is no longer losing SIM card when
receiving or placing phone calls.
Thanks for a tip!
*/
}


void doStatus(void) {

  uint8_t error;
  int numSMS;

  error = 0;
  curStat.FONAOnline = FONAOnline;                                    // First thing, FONA is online or not?
  if (curStat.FONAOnline) {
    if (!fona.getBattVoltage(&curStat.batteryVolts)) {
      curStat.batteryVolts = 0;
      error = error | B00000010;    // noVoltRead;
    }
    if (!fona.getBattPercent(&curStat.batteryPercent)) {
      curStat.batteryPercent = 0;
      error = error | B00000100;    // no percent read;
    }
    curStat.RSSI = fona.getRSSI();
    curStat.networkStat = (networkStatus)fona.getNetworkStatus();
    curStat.volume = fona.getVolume();
    curStat.callStat = (callStatus)fona.getCallStatus();
    numSMS = fona.getNumSMS();
    if (numSMS<0) {
      curStat.numSMSs = 0;
      error = error | B00001000;  // No SMS number ready.
    }
    curStat.numSMSs = numSMS;
    fona.getTime(curStat.networkTime,23);
  } else {
    error = B00000001;            // No FONA online.
  }
  curStat.errByte = error;  

  ourComObj.replyBuff((byte*)&curStat,sizeof(cellStatus));
}


void doPickUp(void) {

  uint8_t result;

  if (fona.pickUp()) {
    result = 0;                   // We'll pass a 0 for no error.
  } else {
    result = 1;                   // We'll pass a 1 for error.
  }
  ourComObj.replyBuff((byte*)&result,sizeof(uint8_t)); 
}


void doCall(char* inNum) {

    uint8_t result;

    if (fona.callPhone(inNum)) {
      result = 0;                   // We'll pass a 0 for no error.
    } else {
      result = 1;                   // We'll pass a 1 for error.
    }
    ourComObj.replyBuff((byte*)&result,sizeof(uint8_t));
    blink(3);
}


void doHangUp(void) {

    uint8_t result;
    
    if (fona.hangUp()) {
      result = 0;           // We'll pass a 0 for no error.
    } else {
      result = 1;           // We'll pass a 1 for error.
    }
    ourComObj.replyBuff((byte*)&result,sizeof(uint8_t));
    blink(3);
}


void doSendSNS(char* inBuff) {
/*
  uint8_t result;
  
  result = 0;
  if (fona.sendSMS(currentPN, inBuff)) {
    result = 1;
  }
  ourComObj.replyBuff((byte*)&result,sizeof(uint8_t));
  */
}
