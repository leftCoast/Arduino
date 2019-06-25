
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

runningAvg smother(10);
mapper moistMap(0,560,0,100);
colorMpper colorMap(0,100,&black,&blue);
 
void setup() {

  pinMode(0, OUTPUT); // motor
  pinMode(1, INPUT);  // Sensor reading
  pinMode(2, OUTPUT); // Not connected
  pinMode(3, OUTPUT); // Not connected
  pinMode(4, OUTPUT); // Sensor power
  

  Serial.begin(57600);
   
}

void loop() {
  
  //idle();
  unsigned long rawValue = analogRead(1);
  float         mappedVal = moistMap.Map(rawValue);
  float         gaugeVal = gaugeMap.Map(mappedVal);
  //gauge.setServo(gaugeVal);
  Serial.println(round(moistMap.Map(smother.addData(analogRead(1)))));
  delay(100);
}
