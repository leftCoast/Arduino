#include "shaker.h"


shaker::shaker(int inShakePin, float sumTime,int numData) {
  
  shakeSum = 0;
  shakePin = inShakePin;
  shakeTimer = new timeObj(sumTime);
  shake = new runningAvg(numData);
}


shaker::~shaker(void) {

    if (shakeTimer) delete(shakeTimer);
    if (shake) delete(shake);
}
 
// Setup the ports etc. 
void shaker::begin(void) {
  
  pinMode(shakePin, INPUT_PULLUP);  // Setup as input.
  hookup();                        // Fire up the idle stuff..
  shakeTimer->start();               // Set the timer.
}


void shaker::idle(void) {
  
  if (!digitalRead(shakePin)) {        // have a shake!
    //while (!digitalRead(shakePin));    // BLOCK 'till clear? How rude!
    shakeSum++;                        // got a shake, add it.
  }
  if (shakeTimer->ding()) {            // time to add the sums to the averagers?
    shake->addData(shakeSum);
    shakeSum = 0;
    shakeTimer->stepTime();
  }
}


float  shaker::result(void) { return shake->getAve(); }

