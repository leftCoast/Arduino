#include <servo.h>

servo testServo(2);

void setup() {
  

}

void loop() {
  idle();
  testServo.setServo(100);
}
