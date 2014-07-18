#include <PinChangeInt.h>
#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>

#include "RCReciver.h"
#include "multiWii.h"

servo theServo(PIN_D46);
reciverPin* theInPin;
mapper servoMpr(1100,1928,-100,100);
blinker blueBlinker(BLUE_LED,200,400);

void setup() {                
  
  //Serial.begin(9600);
  //Serial.println(A8);
  theInPin = new reciverPin(A8);
  theServo.setServo(50);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  blueBlinker.setBlink(true);
}

void loop() {
   
   unsigned long value;
   float servoVal;
   
   theIdlers.idle();
   //theInPin->dataDump();
   value = theInPin->pinResult();
   if (value) {
      servoVal = servoMpr.Map(value);
      theServo.setServo(servoVal);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED,LOW);
   } else {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED,HIGH);
   }
   
}

