#include "timeObj.h"
#include "mapper.h"
//#include "multiMap.h"
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
#define countsPerRev 35  // How many counts per reolution of whatever we're controlling. (Example : wheel)
#define speedTime 100    // ms between rpm calculations.
#define speedGain 500    // How much speed error 'til we hit max power.
#define minPower  7      // 7% power is zero means coast for holding speed.



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
 
timeObj deadMan(20000);

void setup() {

  //Serial.begin(9600);
  chassis.setSteer(80);
  chassis.setRPM(300);
  deadMan.start();
}


void loop() {

  if(!deadMan.ding())
    chassis.idle();

}



