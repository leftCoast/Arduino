#include <LM35.h>
#include <timeObj.h>

timeObj printTimer(1000);    // Gives us a reading every second.
LM35 tempSensor(A4);         // Choose whatever analog pin you like.

void setup() {
    
  Serial.begin(9600);
  
  printTimer.start();
}

void loop() {

  if (printTimer.ding()) {
    Serial.print("Temp C  : ");
    Serial.println(tempSensor.tempC());
    Serial.print("Temp F  : ");
    Serial.println(tempSensor.tempF());
    Serial.println();
    printTimer.stepTime();
  }
}

