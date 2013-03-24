#include "timeObj.h"
#include "mapper.h"
#include "multiMap.h"
#include "chassisObj.h"

// ******** Steering & motor control ********

#define steerPin  6       // tiewrap
#define right     -100
#define left      100
#define straight  0


#define motorPin  7       // No tiewrap
#define motorFwd  -100
#define motorBack 100
#define motorStop 0

#define speedPin  2
#define countsPerRev 35  // How many counts per revolution of whatever we're controlling. (Example : wheel)
#define speedTime 100    // ms between rpm calculations.
#define speedGain 500    // How much speed error 'til we hit max power.
#define minPower  8      // % power used as coast for holding speed.

chassisObj chassis(
 steerPin, 
 right,
 straight,
 left,
 
 motorPin, 
 motorFwd,
 motorStop,
 motorBack,
 
 speedPin,
 countsPerRev,
 speedTime,
 speedGain,
 minPower
 );

// ******** Eye stuff ********

#define rightEye  A5 // White
#define leftEye   A4 // Orange
mapper mapperVolt(0,1023,0,5.0);
multiMap mapperCM;

float distRight;
float distLeft;
float diff;


void setupCmMpr(void) {

  mapperCM.addPoint(2.55,20);
  mapperCM.addPoint(2.0,30);
  mapperCM.addPoint(1.51,40);
  mapperCM.addPoint(1.25,50);
  mapperCM.addPoint(0.85,70);
  mapperCM.addPoint(0.75,90);
  mapperCM.addPoint(0.5,130);
  mapperCM.addPoint(0.45,150);
}


void look(void) {

  int rawRight;
  float voltRight;
  int rawLeft;
  float voltLeft;

  rawRight = analogRead(rightEye);
  voltRight = mapperVolt.Map(rawRight);
  distRight = mapperCM.Map(voltRight);

  rawLeft = analogRead(leftEye);
  voltLeft = mapperVolt.Map(rawLeft);
  distLeft = mapperCM.Map(voltLeft);
  
  diff = distLeft - distRight;
}


// ******** AI stuff ********

enum {
  startstate, seeBoth, seeRight, seeLeft, lostRight, lostLeft, endstate
};

int driveState;

#define slow      200    // RPM
#define fast      500    // RPM
mapper speedMapper(130,0,slow,fast);

#define bothGain 0.5
#define wallGain 0.5
mapper rightWall(50, 150, wallGain*steerLeft, wallGain*steerRight);
mapper leftWall(50, 150, wallGain*steerRight, wallGain*steerLeft);

timeObj blindTurn(1900);
timeObj deadMan(30000);


void setup() {

  Serial.begin(9600);
  setupCmMpr();                     // Cm mapper is a multi mapper. It needs its points.
  chassis.setSteer(steerStraight);  // Defaul steering to straight.
  chassis.setRPM(motorStop);        // Drive motor off.
  deadMan.start();
}


void loop() {

  float steerVal;
  look();

  switch(driveState) {

  case startstate:
    if(distRight < 150 && distLeft < 150){
      driveState = seeBoth;
    }
    else if(distRight < 150){
      driveState = seeRight;
    }
    else if(distLeft < 150){
      driveState = seeLeft;
    }
    chassis.setSteer(steerStraight);
    chassis.setRPM(motorStop); // Drive motor off.
    break;


  case seeBoth:
    if(distRight < 150 && distLeft < 150){
      steerVal = bothGain*diff;
      chassis.setSteer(steerVal);
      chassis.setRPM(speedMapper.Map(abs(steerVal)));
    }
    else if(distLeft == 150){
      driveState = seeRight;
    }
    else if(distRight == 150){
      driveState = seeLeft;
    }
    break;


  case seeRight:
    if(distRight < 150 && distLeft < 150){
      driveState = seeBoth;
    }
    else if(distRight == 150){
      driveState = lostRight;
      blindTurn.start();
    }
    else{
      steerVal = rightWall.Map(distRight);
      chassis.setSteer(steerVal);
      chassis.setRPM(speedMapper.Map(abs(steerVal)));
    }
    break;


  case seeLeft:
    if(distRight < 150 && distLeft < 150){
      driveState = seeBoth;
    } 
    else if(distLeft == 150){
      driveState = lostLeft;
      blindTurn.start();
    }
    else{
      steerVal = leftWall.Map(distLeft);
      chassis.setSteer(steerVal);
      chassis.setRPM(speedMapper.Map(abs(steerVal)));
    }    
    break;


  case lostRight:
    if(distRight < 150 && distLeft < 150){
      driveState = seeBoth;
    }
    else if(distRight < 150){
      driveState = seeRight;
    }
    else if(distLeft < 150){
      driveState = seeLeft;
    }
    else{ 
      if (!blindTurn.ding()) {
        chassis.setSteer(steerRight);
        chassis.setRPM(slow);
      } 
      else{
        chassis.setSteer(steerStraight);
      }
    }
    break;


  case lostLeft:
    if(distRight < 150 && distLeft < 150){
      driveState = seeBoth;
    }
    else if(distRight < 150){
      driveState = seeRight;
    }
    else if(distLeft < 150){
      driveState = seeLeft;
    }
    else{
      if (!blindTurn.ding()) {
        chassis.setSteer(steerLeft);
        chassis.setRPM(slow);
      }
      else{
        chassis.setSteer(steerStraight);
      }
    }
    break;

  case endstate:
    break;
  }

  if(deadMan.ding()) {
    chassis.motorOff();
  }
  chassis.idle();
}


