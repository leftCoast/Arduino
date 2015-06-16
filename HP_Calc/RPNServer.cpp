#include "remoteRPNCalc.h"
#include "RPNServer.h"

RPNServer::RPNServer(void) { }
RPNServer::~RPNServer(void) { }
  
void RPNServer::init(void) {
  
  Serial.begin(REMOTE_RPN_BAUD);
}


// Grabs the values and spits out the data.
void RPNServer::sendReply(void) {
  
  xReg_Fix outBuff;
  byte numBytes = sizeof(xReg_Fix);
  xReg_Fix.xReg = getX();
  xReg_Fix.fix = getFixVal();
  char* buffPtr = (char*)&xReg_Fix;
  Serial.write(buffPtr,numBytes);
};


// Blocks for a c string;  
void RPNServer::geyKeyPress(void) {
  
  boolean  done = false;
  byte index = 0;
  while(!done) {
    if (Serial.available()) {
      keyBuf[index] = Serial.read();
      index++;
      if (keyBuf[index]=='0') {
        buttonClick(keyBuf);
        sendReply();
        done = true;
      }
    }
  }
}
