#include "tapSensor.h"

#define SENSOR_PIN 15

tapSensor aSensor(SENSOR_PIN);
timeObj  writeTimer(40);

void setup() {
   
   aSensor.begin();
   writeTimer.start();
}

void loop() {

   idle();
   if (writeTimer.ding()) {
      Serial.print(0);
      Serial.print(" 100 ");
      if (aSensor.getTapVal()>10) {
         Serial.println(50);
      } else {
         Serial.println(5);
      }
      writeTimer.start();
   }
}
