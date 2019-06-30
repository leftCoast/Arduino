
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

void setup() {

  pinMode(0, OUTPUT); //
  pinMode(1, OUTPUT); //
  pinMode(2, INPUT_PULLUP);  // Motor button
  pinMode(3, OUTPUT); // IN2 Motor
  digitalWrite(3, LOW);
  pinMode(4, OUTPUT); // IN1 Motor
  digitalWrite(4, LOW);

  Serial.begin(57600);

}

void loop() {

  digitalWrite(3, !digitalRead(2));
}
