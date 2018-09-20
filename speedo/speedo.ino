#include "mapper.h"
#include "idlers.h"
#include "timeObj.h"
#include "servo.h"

#define MIN_TIME  500
#define MAX_TIME  2000

timeObj changeTime;
servo   mServo(2);

float randomTime;

void setup() {
  mServo.setServo(100);
  //while(true) idle();
  randomTime = random(MIN_TIME,MAX_TIME);
  changeTime.setTime(randomTime);
}

void loop() {
  
  
  float newPosition;
  
  idle();
  if(changeTime.ding()) {
    newPosition = (random(0,2000)/10.0)-100;
    mServo.setServo(newPosition);
    randomTime = random(MIN_TIME,MAX_TIME);
    changeTime.setTime(randomTime);
  }
}
