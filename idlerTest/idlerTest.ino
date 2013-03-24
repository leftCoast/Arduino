
#include "blinker.h"
#include "servo.h"


servo   mServo(2);
timeObj servoTimer(300);
blinker mBlinker;

int side;

void setup() {
  Serial.begin(9600);
  mBlinker.setBlink(true);
  side = 1;
}


void loop() {
  idle();
  if (servoTimer.ding()) {
    if (side==1)
      mServo.setServo(100);
    else
      mServo.setServo(-100);
    servoTimer.stepTime();
    side = side * -1;
  }
}

