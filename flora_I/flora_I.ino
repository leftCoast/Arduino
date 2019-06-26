
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
//colorMpper colorMap(0,100,&black,&blue);
 
void setup() {

  pinMode(0, OUTPUT); // motor
  digitalWrite(0, LOW); // motor off.
  
  //pinMode(1, INPUT);  // Sensor reading
  pinMode(2, OUTPUT); // Not connected
  pinMode(3, OUTPUT); // Not connected
  pinMode(4, OUTPUT); // Sensor power
  digitalWrite(4,LOW); // Power up the sensor.
  

  Serial.begin(57600);
   
}

void loop() {
  
  unsigned long rawValue = analogRead(A0);
  //float         mappedVal = moistMap.Map(rawValue);
  Serial.print(rawValue);Serial.print(" ,");
  Serial.println(round(moistMap.Map(smother.addData(analogRead(A0)))));
  
  delay(500);
  digitalWrite(0, LOW);
  Serial.println("motor off.");
  delay(500);
  digitalWrite(0, HIGH);
  Serial.println("motor on.");
}
