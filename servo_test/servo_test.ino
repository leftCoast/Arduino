#include "servo.h"
#include "timeObj.h"

servo servo1(7);
timeObj myTimer(2000);
float servoVal = 100;

void setup() {

  servo1.setServo(servoVal);
  myTimer.start();
}

void loop() {

  if (myTimer.ding()) {
    myTimer.stepTime();
    servoVal = -servoVal;
    servo1.setServo(servoVal);
  }
  servo1.idle();
}

