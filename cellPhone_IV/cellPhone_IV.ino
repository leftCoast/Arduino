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

#define COM_BUFF_BYTES  255




SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

bool          FONAOnline;
qCSlave       ourComObj;
byte          comBuff[COM_BUFF_BYTES]; // Buffer for all comunication.
byte*         comPtr;


void setup() {

    pinMode(0,INPUT);                               // Adafruit says to do this. Otherwise it may read noise.
    pinMode(13, OUTPUT);                            // Our one and only debugging tool.
    ourComObj.begin(comBuff,COM_BUFF_BYTES,9600);   // Buff, bytes & Baud. Setup for talking to the GUI.
    checkComErr();
    pinMode(FONA_RST, OUTPUT);            // Used for resetting the FONA.
    FONAOnline = false;
    resetFONA();                          // Hit reset, see if it'll come online.
}


void blink(int numBlinks) {

  if (numBlinks==1) { 
    digitalWrite(13, HIGH);
    delay(75);
    digitalWrite(13, LOW);
    delay(75);
    return;
  }
  for (int i=0;i<numBlinks;i++) {
    digitalWrite(13, HIGH);
    delay(75);
    digitalWrite(13, LOW);
    delay(150);
  }
}


void checkComErr() {
  int error;
  error = ourComObj.readErr();
 if (!error==NO_ERR) {    // Did the poor thing fire up?
    while(true) {
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);
      delay(1000);
      blink(error);
      delay(1000);
    }
  }  
}


void loop() {
  
  idle();
  checkComErr();
  if (ourComObj.haveBuff()) {
    comPtr = ourComObj.getComBuff();
    switch(comPtr[0]) {
      case getStatus    : doStatus(comPtr);   break;
      case makeCall     : doCall(comPtr);     break;  
      case hangUp       : doHangUp(comPtr);   break;
      case sendSNS      : doSendSNS(comPtr);  break;
      case pickUp       : doPickUp(comPtr);   break;
      default           : break;
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


void doStatus(byte* buff) {

  cellStatus* statPtr;
  int         error;
  int         numSMS;

  statPtr = (cellStatus*)buff;                                      // Basically lay a template over the com buffer.
  error = 0;                                                        // No erors yet.
  statPtr->FONAOnline = FONAOnline;                                 // First thing. Is FONA online or not?
  if (statPtr->FONAOnline) {                                        // If FONA is online..
    if (!fona.getBattVoltage(&statPtr->batteryVolts)) {             // Attempt to read the battery voltage..
      statPtr->batteryVolts = 0;                                    // If there was an error, send out 0 volts.
      error = error | B00000010;                                    // Set the error bit for our error flag.
    }
    if (!fona.getBattPercent(&statPtr->batteryPercent)) {           // Attmept to read the battery percent value.
      statPtr->batteryPercent = 0;                                  // If there was an error, send out 0 percent.
      error = error | B00000100;                                    // And set the error bit for our error flag.
    }
    statPtr->RSSI = fona.getRSSI();                                 // Read RSSI, no error checking available here.
    statPtr->networkStat = (networkStatus)fona.getNetworkStatus();  // Read network status. Same, no error checking.  
    statPtr->volume = fona.getVolume();                             // Volume.
    statPtr->callStat = (callStatus)fona.getCallStatus();           // Call status, no error checking.
    numSMS = fona.getNumSMS();                                      // Number SMS(s). This one gives bogus numbers at times.
    if (numSMS<0) {                                                 // Try to catch the errors but might not work.
      statPtr->numSMSs = 0;                                         // Bogus readings get zero.
      error = error | B00001000;                                    // Flag the error.
    }
    statPtr->numSMSs = numSMS;                                      // Send on what we found.
    fona.getTime(statPtr->networkTime,23);                          // Write out the network time string.
  } else {
    error = B00000001;                                              // If the FONA was offline, flag the eror.
  }
  statPtr->errByte = error;                                         // And finally the error flag goes in.
  ourComObj.replyComBuff(sizeof(cellStatus));                      // Send the info on its way.
}


void doPickUp(byte* buff) {

  if (fona.pickUp()) {
    buff[0] = 0;                   // We'll pass a 0 for no error. (We got the call.)
  } else {
    buff[0] = 1;                   // We'll pass a 1 for error. (We missed the call.)
  }
  ourComObj.replyComBuff(1); 
}


void doCall(byte* buff) {
  
  if (fona.callPhone((char*)&buff[1])) {
    buff[0] = 0;                   // We'll pass a 0 for no error.
  } else {
    buff[0] = 1;                   // We'll pass a 1 for error.
  }
  ourComObj.replyComBuff(1);
}


void doHangUp(byte* buff) {
    
  if (fona.hangUp()) {
    buff[0] = 0;                   // We'll pass a 0 for no error.(We hung up)
  } else {
    buff[0] = 1;                   // We'll pass a 1 for error.(Help! We can't hang up!)
  }
  ourComObj.replyComBuff(1);
}


void doSendSNS(byte* buff) {
/*
  uint8_t result;
  
  result = 0;
  if (fona.sendSMS(currentPN, inBuff)) {
    result = 1;
  }
  ourComObj.replyBuff((byte*)&result,sizeof(uint8_t));
  */
}
