
#include "multiMap.h"
#include "lists.h"
#include "timeObj.h"
#include "squareWave.h"
#include "baseTrapMove.h"
#include "RCServoObj.h"


#include <Servo.h>

RCServoObj aServo(2);
timeObj timer(1500);

//Servo myservo;

void setup() {

   Serial.begin(57600);
   Serial.println("Hello..");
   //myservo.attach(2);
   //myservo.write(5);
   
   
   aServo.move(0);
   timer.start();
   while(!timer.ding()) idle();
   
}

void loop() {

   idle();
   aServo.controlledMove(0,100,.01);
   while(aServo.moving()) idle();
   aServo.controlledMove(100,100,.01);
   while(aServo.moving()) idle();

   aServo.controlledDegreeMove(45,100,.150);
   while(aServo.moving()) idle();
   aServo.controlledDegreeMove(90,100,.150);
   while(aServo.moving()) idle();
  delay(1000);
}
