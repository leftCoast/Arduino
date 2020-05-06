#include "tapSensor.h"

#define SENSOR_PIN 15

tapSensor aSensor(SENSOR_PIN);
timeObj  writeTimer(250);
void setup() {
   
   //aSensor.begin();
   pinMode(SENSOR_PIN, INPUT);         // Setup as input.
   digitalWrite(SENSOR_PIN, HIGH);
   writeTimer.start();
}

void loop() {
   if (!digitalRead(SENSOR_PIN)) {
      Serial.println("got a low");
   }
   /*
   idle();
   if (writeTimer.ding()) {
      Serial.println(aSensor.getTapVal());
      writeTimer.start();
   }
   */
}
