#include <RCReciver.h>
#include <servo.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <timeObj.h>

servo gauge(2);
runningAvg smother(10);
mapper moistMap(0,560,0,100);
mapper gaugeMap(0,100,-100,100);
 
void setup() {

  Serial.begin(57600);   
  gauge.setServo(-100);
}

void loop() {
  
  idle();
  unsigned long rawValue = analogRead(A0);
  float         mappedVal = moistMap.Map(rawValue);
  float         gaugeVal = gaugeMap.Map(mappedVal);
  gauge.setServo(gaugeVal);
  //Serial.println(round(moistMap.Map(smother.addData(analogRead(A0)))));
  //delay(100);
}
