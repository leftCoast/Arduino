
#include "servo.h"

servo myServo(2);

void setup() {
  myServo.init();
  Serial.begin(9600);
}


void loop() {

  byte inByte;

  myServo.idle();
  if (Serial.available() > 0) {
    inByte = Serial.read();
    switch (inByte) {
    case '+' :
      myServo.setServo(100);
      Serial.println("Maximum");
      break;
    case '-' :
      myServo.setServo(-100);
      Serial.println("Minimum");
      break;
    case '0' :
      myServo.setServo(0);
      Serial.println("Zero");
      break;
    case '\n' :
      break;
    default :
      Serial.println("This runs an R/C servo off Pin 2");
      Serial.println("Enter '+' for max, '-' for min or '0' for center.");
    }
  }
}


