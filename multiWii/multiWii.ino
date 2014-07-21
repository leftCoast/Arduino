#include <Wire.h>
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
#include "gyro.h"
#include "accell.h"
#include "multiWii.h"
//#include "barometer.h"

servo theServo(PIN_D46);
reciverPin* theInPin;
mapper servoMpr(1100,1928,-100,100);
blinker blueBlinker(BLUE_LED,200,400);
gyro* theGyro;
accell* theAccell;

void setup() {                

   Serial.begin(9600);
   Wire.begin();
   theInPin = new reciverPin(A8);
   theGyro = new gyro();
   theAccell = new accell();
   theServo.setServo(50);
   pinMode(RED_LED, OUTPUT);
   pinMode(GREEN_LED, OUTPUT);
   pinMode(AMBER_LED, OUTPUT);
   //blueBlinker.setBlink(true);
}

void loop() {

   unsigned long value;
   float servoVal;
   
   int  xVal;
   int  yVal;
   int  zVal;
  
   //delay(100);
   digitalWrite(AMBER_LED, LOW);         // yellow on.
   theIdlers.idle();
  
   if (theGyro->newReadings()) {
      theGyro->readValues(&xVal,&yVal,&zVal);
      Serial.println();
      Serial.print("x rotation = ");Serial.println(xVal);  // print the values
      Serial.print("y rotation = ");Serial.println(yVal);
      Serial.print("z rotation = ");Serial.println(zVal);
      Serial.println();
   }
   
   if (theAccell->newReadings()) {
      theAccell->readValues(&xVal,&yVal,&zVal);
      Serial.print("x accell  = ");Serial.println(xVal);  // print the values
      Serial.print("y accell  = ");Serial.println(yVal);
      Serial.print("z accell  = ");Serial.println(zVal);
      Serial.println();
   }//else
   //theAccell->dataDump();
   
   digitalWrite(AMBER_LED, HIGH);         // yellow off.

   value = theInPin->pinResult();
   if (value) {
      servoVal = servoMpr.Map(value);
      theServo.setServo(servoVal);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED,LOW);
   } 
   else {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED,HIGH);
   }



}


