#include "serialHandler.h"

#define  DEF_HOLD_SEC   2                 // Wait at least this many before sending again.
#define  PORT_HOLD_MS   40                // Wait at least this long after a transmisson.


timeObj        portTimer(PORT_HOLD_MS);   // Redraw takes about 36 miliseconds. We'll wait a little more between messages.
serialHandler* nextHandler = NULL;        // If it's NULL, grab it to get in line.
int            portBytes    = 0;           // First one that sees this zero will set it to the port buffer size.

serialHandler::serialHandler(netObj* inNetObj,Stream* inPort)
   : msgHandler(inNetObj) {

   ourPort = inPort;
   ourHoldTimer.setTime(DEF_HOLD_SEC * 1000.0);
   lastValue = NAN;
   ourState = watchForChange;
   if (portBytes==0) {
      portBytes = inPort->availableForWrite();
   }
   blindSendTimer.setTime(60*1000); // set it for a minute?
}


serialHandler::~serialHandler(void) {  }


void  serialHandler::idleTime() {
   
   msgHandler::idleTime();
  //Serial.println((int)ourState);
   switch(ourState) {
     
      case doingHoldTime   :              // We're waiting 'till it's time to check values again.
         //Serial.println("doingHoldTime");
         if (ourHoldTimer.ding()) {       // If the timer goes "ding", time's up!
            ourHoldTimer.reset();         // Turn off the timer.
            ourState = watchForChange;    // Now we're waiting to see if there is a change of value.
         }                                //
      break;                              //
      case watchForChange   :             // Watching to see a change in valuue.
         if (valueChanged()||blindSendTimer.ding()) { // If we detected a change in value..
            blindSendTimer.start();       // In all cases, reset the blind send timer.
            ourState = claimingPort;      // It's time to work on getting the port.
         }                                //
      break;                              //
      case claimingPort     :             // Now we're watching the port to grab it when we can.
         if (nextHandler==NULL) {         // If the port is NULL..
            nextHandler = this;           // We grab it for ourselves.
            ourState = waitPortReady;     // Now it's time to wait for the port to be ready to use.
         }                                //
      break;                              //
      case waitPortReady   :              // Waiting for the port to be ready to use.
         if (portTimer.ding()) {          // If the port timer goes "ding", port's ready!
            portTimer.reset();            // Shut down the timer..
            sendMessage();                // Pack up and send our message.
            ourHoldTimer.start();         // Start the hold timer. (Hold time between OUR messages)
            ourState = waitPortDone;      // We are now waiting for our message to complete sending.
         }                                //
      break;                              //
      case waitPortDone    :              // Waiting for our message to clear out of the sending buffer.
         if (xMissionDone()) {            // If the message is all cleard out..
            portTimer.start();            // Start the hold timer. (Minimum time between ANY message)
            nextHandler = NULL;           // Release the port.
            ourState = doingHoldTime;     // We are back to waiting for our holding time to expire.
         }                                // And that's-
      break;                              // What we-
   }                                      // Do.
}

// Default?
bool  serialHandler::valueChanged(void) { return false; }


bool  serialHandler::xMissionDone(void) {
   
   //Serial.print(portBytes);
   //Serial.print("\t");
   //Serial.println(ourPort->availableForWrite());
   return portBytes <= ourPort->availableForWrite();
 }


void  serialHandler::sendMessage(void) {  }
