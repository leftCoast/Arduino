
#include "chassisObj.h"
#include "mapper.h"
#include "servo.h"

// ******** Control mapers ********

mapper steerMapper(1,2,1,2);   // Dummy values
mapper fwdPwrMapper(1,2,1,2);
mapper backPwrMapper(1,2,1,2);
mapper fwdSpdPwrMapper(1,2,1,2);
mapper backSpdPwrMapper(1,2,1,2);


// ******** RPM stuff ********

volatile unsigned long savedMicros0 = 0;
volatile unsigned long lastMicros0 = 0;
volatile unsigned long count0 = 0;

volatile unsigned long savedMicros1 = 0;
volatile unsigned long lastMicros1 = 0;
volatile unsigned long count1 = 0;

void speedInt0(void) {

  count0++;
  savedMicros0 = lastMicros0;
  lastMicros0 = micros();
}

void speedInt1(void) {

  count1++;
  savedMicros1 = lastMicros1;
  lastMicros1 = micros();
}


enum { 
  power, rpm };

chassisObj::chassisObj(
int inSteerPin, 
int inRight,
int inStraight,
int inLeft,

int inMotorPin, 
int inFwd,
int inStop,
int inBack,

int inSpeedPin,
int inCountsPerRev,
float inSpeedTime,
float speedGain,
float inMinPower
) {
  
  steer = new servo(inSteerPin);
  motor  = new servo(inMotorPin);
  
  speedPin = inSpeedPin;
  countsPerRev = inCountsPerRev;

  steerMapper.setValues(-100,100,inLeft,inRight);
  fwdPwrMapper.setValues(0,100,inStop,inFwd);
  backPwrMapper.setValues(0,-100,inStop,inBack);
  fwdSpdPwrMapper.setValues(0,speedGain,fwdPwrMapper.Map(inMinPower),fwdPwrMapper.Map(100));
  backSpdPwrMapper.setValues(0,speedGain,backPwrMapper.Map(-inMinPower),backPwrMapper.Map(-100));

  motorEnable = false;
  reverse = false;
  motorMode = power;
  
  if (speedPin == 2) {
    pinMode(speedPin,INPUT);                // We're using this pin for speed sensor.
    digitalWrite(speedPin,HIGH);            // And it needs its internal pull ups set.
    attachInterrupt(0, speedInt0, RISING);  // We want inturupts on rising pulses.    
    speedCheck = new timeObj(inSpeedTime);
    speedCheck->start();
  } 
  else if (speedPin == 3) {                 // Or this pin..
    pinMode(speedPin,INPUT);               
    digitalWrite(speedPin,HIGH);
    attachInterrupt(1, speedInt1, RISING);  
    speedCheck = new timeObj(inSpeedTime);
    speedCheck->start();
  }   
}


void chassisObj::setSteer(float steerVal) { 
  steer->setServo(steerMapper.Map(steerVal)); 
}


void chassisObj::setPower(float powerVal) {

  if (powerVal==0) {
    motorEnable = false;
  } 
  else {
    if (powerVal>0) {
      motor->setServo(fwdPwrMapper.Map(powerVal));
    } 
    else {
      motor->setServo(backPwrMapper.Map(powerVal));
    }
    motorEnable = true;
  }
  motorMode = power;
}


void chassisObj::motorOff(void) { 
  motorEnable = false; 
}


void chassisObj::setRPM(float inRPMVal) {

  if (speedPin == 2 || speedPin == 3) {
    rpmVal = inRPMVal;
    Serial.print("SetRPM : ");Serial.println(inRPMVal);
    if (rpmVal==0) {
      motorEnable = false;
      reverse = false;              // If you think about it, we're not going backwards.
    } else {
      reverse = rpmVal < 0;
      motorEnable = true;
    }
    motorMode = rpm;
  }
}


float chassisObj::readRPM(void) {

  long diffMicros;

  if (speedPin == 2) 
    diffMicros = lastMicros0 - savedMicros0;
  else 
    diffMicros = lastMicros1 - savedMicros1;
  return 60000000.0/(diffMicros * countsPerRev);
}


void chassisObj::setDistance(long counts) { 

  if (speedPin == 2)
    count0 = counts;
  else
    count1 = counts;
}


long chassisObj::getDistance(void) {

  if (speedPin == 2)
    return count0;
  else
    return count1;
}


void chassisObj::idle(void) {

  float powerVal;
  float speedError;

  switch (motorMode) {

  case power :
    // nothing to do here..
    break;

  case rpm :
    if (speedCheck->ding()) {
      speedError = rpmVal - readRPM();
      Serial.print("RPM : "); Serial.print(readRPM());
      Serial.print("  Speed error : "); Serial.print(speedError);
      if (reverse) {
        Serial.println(" Going backwards ");
        backSpdPwrMapper.printMap();
        powerVal = backSpdPwrMapper.Map(speedError);
      } else {
        Serial.println(" Going forwards ");
        fwdSpdPwrMapper.printMap();
        powerVal = fwdSpdPwrMapper.Map(speedError);
      }
      Serial.print(" power : ");Serial.println(powerVal);
      
      motor->setServo(powerVal);
      speedCheck->stepTime();
    }
    break;
  }
  if (motorEnable) 
    motor->idle();
  steer->idle();
}



















