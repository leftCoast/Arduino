#include <idlers.h>
#include <lists.h>
#include <quickCom.h>
#include "cellCommon.h"

#include <SoftwareSerial.h>

#define COM_BUFF_BYTES  255

qCSlave     ourComObj;
byte        comBuff[COM_BUFF_BYTES]; // Buffer for all comunication.
byte*       comPtr;

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


void setup() {
  
  pinMode(0,INPUT);                     // Adafruit says to do this. Otherwise it may read noise.
  pinMode(13, OUTPUT);
  ourComObj.begin(comBuff,COM_BUFF_BYTES,9600);                // For talking to the GUI.
  checkComErr();                        
}


void loop() {
  checkComErr();
  idle();
  if (ourComObj.haveBuff()) {
    digitalWrite(13, HIGH);
    comPtr = ourComObj.getComBuff();
    switch(comPtr[0]) {
      case getStatus    : doStatus(comPtr);   break;
      case makeCall     : doCall(comPtr);     break;  
      case hangUp       : doHangUp(comPtr);   break;
      case sendSNS      : doSendSNS(comPtr);  break;
      case pickUp       : doPickUp(comPtr);   break;
      default           : break;
    } 
    digitalWrite(13, LOW);
    delay(100);
  }
}


void doStatus(byte* comPtr) {

  cellStatus* statPtr;
  blink(3);
  
  statPtr = (cellStatus*)comPtr;                                   // Basically lay a template over the com buffer.
  statPtr->FONAOnline = 1;                                     // First thing. Is FONA online or not?
  statPtr->batteryVolts = 0;                                    // If there was an error, send out 0 volts.
  statPtr->batteryPercent = 0;                                  // If there was an error, send out 0 percent.
  statPtr->RSSI = 2;                                 // Read RSSI, no error checking available here.
  statPtr->networkStat = 2;  // Read network status. Same, no error checking.  
  statPtr->volume = 32;                             // Volume.
  statPtr->callStat = 0;           // Call status, no error checking.
  statPtr->numSMSs = 12;                                      // Send on what we found.
  strcpy((char*)statPtr->networkTime,"message");
  statPtr->errByte = 200; 
  ourComObj.replyComBuff(sizeof(cellStatus));                      // Send the info on its way.
}


void doPickUp(byte* buff) {

  delay(2000);
}


void doCall(byte* buff) {
  
 delay(3000);
}


void doHangUp(byte* buff) {
    
 delay(4000); 
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
