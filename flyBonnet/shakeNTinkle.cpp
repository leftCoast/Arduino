#include "shakeNTinkle.h"


shakeNTinkle::shakeNTinkle(int inShakePin,int inTinklePin,float sumTime,int numData) {
  
  shakeSum = 0;
  tapSum = 0; 
  shakePin = inShakePin;
  tinklePin = inTinklePin;
  tapTimer = new timeObj(sumTime);
  shake = new runningAvg(numData);
  tinkle = new runningAvg(numData);
}


shakeNTinkle::~shakeNTinkle(void) {
  
    if (tapTimer) delete(tapTimer);
    if (shake) delete(shake);
    if (tinkle) delete(tinkle);
}
 
// Setup the ports etc. 
void shakeNTinkle::begin(void) {
  
  pinMode(shakePin, INPUT);        // Setup as input.
  digitalWrite(shakePin, HIGH);    // Pullups on!
  pinMode(tinklePin, INPUT);       // Setup as input.
  digitalWrite(tinklePin, HIGH);   // Pullups on!
  hookup();                        // Fire up the idle stuff..
  tapTimer->start();               // Set the timer.
}


void shakeNTinkle::idle(void) {
  
  if (!digitalRead(shakePin)) {        // have shake!
    while (!digitalRead(shakePin));    // hold 'till clear.
    shakeSum++;                        // got a shake, add it.
  }
  if (!digitalRead(tinklePin)) {          // have tap!
    while (!digitalRead(tinklePin));      // hold 'till clear.
    tapSum++;                             // got a tap, add it.
  }
  if (tapTimer->ding()) {               // time to add the sums to the averagers?
    shake->addData(shakeSum);
    tinkle->addData(tapSum);
    shakeSum = 0;
    tapSum = 0;
    tapTimer->stepTime();
  }
}


float  shakeNTinkle::getShake(void) { return shake->getAve(); }

float  shakeNTinkle::getTinkle(void) { return tinkle->getAve(); }

//void   shakeNTinkle::clearShake(void) { shakeSum = 0; }

