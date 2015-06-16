#include "arduino.h"
#include "remoteRPNCalc.h"
#include "RPNClient.h"


RPNClient::RPNClient(void) { }
RPNClient::~RPNClient(void) { }
  
void RPNClient::begin(void) { Serial.begin(REMOTE_RPN_BAUD); }


void RPNClient::buttonClick(char* buttonID) {
  
  xReg_Fix  theReg;
  boolean  done = false;
  Serial.write(buttonID);
  while(!done) {
    if (Serial.available()>=sizeof(xReg_Fix)) {
      Serial.read((char*)&theReg,sizeof(xReg_Fix));
      done = true;
    }
  }
}


byte RPNClient::getFixVal(void) { return theReg.fix; }


double RPNClient::getX(void) { return theReg.xReg; }
