#include "tapSensor.h"

#define SENSOR_PIN 15

tapSensor aSensor(SENSOR_PIN);
timeObj  writeTimer(20);

void setup() {
   
   aSensor.begin();
   writeTimer.start();
}

void loop() {

   idle();
   if (writeTimer.ding()) {
      Serial.print(0);
      Serial.print(" 100 ");
      Serial.println(aSensor.getTapVal());
      writeTimer.start();
   }
}
