#include "serialHandler.h"

#define  DEF_HOLD_SEC   2                 // Wait at least this many before sending again.
#define  PORT_HOLD_MS   40                // Wait at least this long after a transmisson.


timeObj        portTimer(PORT_HOLD_MS);   // Redraw takes about 36 miliseconds. We'll wait a little more between messages.
serialHandler* nextHandler = NULL;        // If it's NULL, grab it to get in line.


serialHandler::serialHandler(netObj* inNetObj,Stream* inPort)
   : msgHandler(inNetObj) {

     ourPort = inPort;
     ourHoldTimer.setTime(DEF_HOLD_SEC * 1000.0);
     lastValue = NAN;
     ourState = waitForChange;
}


serialHandler::~serialHandler(void) {  }


void  serialHandler::idleTime() {
   
   msgHandler::idleTime();
   switch(ourState) {
      case spendHoldTime   :
         if (ourHoldTimer.ding()) {
            ourHoldTimer.reset();
            ourState = waitForChange;
         }
      break;
      case waitForChange   :
         if (valueChanged()) {
            ourState = claimingPort;
         }
      break;
      case claimingPort     :
         if (!nextHandler) {
            nextHandler = this;
            ourState = waitPortReady;
         }
      break;
      case waitPortReady   :
         if (portTimer.ding()) {
            portTimer.reset();
            sendMessage();
            ourHoldTimer.start();
            ourState = waitPortDone;
         }
      break;
      case waitPortDone    :
         if (xMissionDone()) {
            portTimer.start();
            nextHandler = NULL;
            ourState = spendHoldTime;
         }
      break;
   }  
}


bool  serialHandler::valueChanged(void) {  }


bool  serialHandler::xMissionDone(void) { return !ourPort->availableForWrite(); }


void  serialHandler::sendMessage(void) {  }
