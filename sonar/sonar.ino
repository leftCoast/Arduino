#include "sonarObj.h"

sonarObj sonar(3);
float distance = -1;

void setup(){
  Serial.begin(9600);
}

void loop() {

  float temp;

  sonar.idle();
  temp = sonar.distanceCm();
  if (temp!=distance) {
    distance = temp;
    Serial.print("distance : "); Serial.println(distance);
  }
}


