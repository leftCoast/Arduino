#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <timeObj.h>

blinker myBlinker;
timeObj timer(500);
void setup() {
  
  Serial.begin(9600);
  myBlinker.setBlink(true);
  timer.start();
}

void loop() {
  idle();
  if (timer.ding()) {
    Serial.print("Help!!");
    timer.stepTime();
  } 
}
