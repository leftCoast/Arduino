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
#include "runningAvg.h"

servo theServo(PIN_D46);
reciverPin* theInPin;
mapper servoMpr(1100,1928,-100,100);
blinker blueBlinker(BLUE_LED,200,400);
gyro* theGyro;
accell* theAccell;
long iteration;

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
   iteration = 0;
   delay(15000);   // time to get the screen up.
}


void loop() {

   unsigned long value;
   float servoVal;
   
   int  xVal;
   int  yVal;
   int  zVal;
   int x_offset;
   int y_offset;
   int z_offset;
   
   digitalWrite(AMBER_LED, LOW);         // yellow on.
   theIdlers.idle();
   
   if (iteration==40) {
      Serial.println("Calibrating..");
      Serial.println("Calibrating..");
      Serial.println("Calibrating..");
      Serial.println("Calibrating..");
      theGyro->calibrate();
      theGyro->setAngles(0,0,0);        //Ok, lets assume we're flat..
   }
   if (theGyro->newReadings()) {
      theGyro->readValues(&xVal,&yVal,&zVal);
      Serial.println();
      Serial.print("x Rotation = ");Serial.println(xVal);  // print the values
      Serial.print("y Rotation = ");Serial.println(yVal);
      Serial.print("z Rotation = ");Serial.println(zVal);
      Serial.println();
      theGyro->readAngles(&xVal,&yVal,&zVal);
      Serial.println();
      Serial.print("x Angle = ");Serial.println(xVal);  // print the values
      Serial.print("y Angle = ");Serial.println(yVal);
      Serial.print("z Angle = ");Serial.println(zVal);
      Serial.println();
   }
   /*
   if (theAccell->newReadings()) {
      theAccell->readValues(&xVal,&yVal,&zVal);
      Serial.print("x accell  = ");Serial.println(xVal);  // print the values
      Serial.print("y accell  = ");Serial.println(yVal);
      Serial.print("z accell  = ");Serial.println(zVal);
      Serial.println();
   }
   */
   iteration++;
   
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


