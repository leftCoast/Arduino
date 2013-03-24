#include "antThorax.h"

// Start with leg code..

antLeg::antLeg(antThorax* inThorax, byte inLegNum, byte inSwivel, byte inLift, byte inKnee) {

  thorax = inThorax;
  legNum = inLegNum;
  if (legNum==0||legNum==2||legNum==4)
    mult = -1;              // port legs
  else
    mult = 1;              // starboard legs
  swivelPin = inSwivel;
  liftPin = inLift;
  kneePin = inKnee;
  legTimer = new timeObj(CYCLE_TIME/SLICES);
  legState = unknown;
}


void antLeg::walkFwd() {

  float mappedTime;

  switch (legState) {
  case walkingFwd :
    if (legTimer->ding()) {
      legTimer->stepTime();                  // reset the timer..

      // send step data.
      thorax->theSSC.setServo(swivelPin,swivelVal);    // Send out the data.
      thorax->theSSC.setServo(liftPin,liftVal);
      thorax->theSSC.setServo(kneePin,kneeVal);

      timeSlice++;                          // calculate next step.
      if (timeSlice>SLICES)                 // Overrun number of slices?
        timeSlice = 0;                      // reset the silly thing.
      mappedTime = thorax->mapTime(timeSlice);
      swivelVal = mult * thorax->legsMapPoint(legNum,swivel,mappedTime);
      liftVal = mult * thorax->legsMapPoint(legNum,lift,mappedTime);
      kneeVal = mult * thorax->legsMapPoint(legNum,knee,mappedTime);
    }
    break;
  default :
    timeSlice = 0;
    mappedTime = thorax->mapTime(timeSlice);
    swivelVal = mult * thorax->legsMapPoint(legNum,swivel,mappedTime);
    liftVal = mult * thorax->legsMapPoint(legNum,lift,mappedTime);
    kneeVal = mult * thorax->legsMapPoint(legNum,knee,mappedTime);
    legState = walkingFwd;
    legTimer->start();
    break;
  } 
}


void antLeg::fold(void) {

  if (legState != folded) {
    thorax->theSSC.setServo(swivelPin,swivleFold*mult);
    delay(100);
    thorax->theSSC.setServo(liftPin,liftFold*mult);
    delay(100);
    thorax->theSSC.setServo(kneePin,kneeFold*mult);
    legState = folded;
    delay(100);
  }
}


void antLeg::park(void) {

  if (legState != parked) {
    thorax->theSSC.setServo(swivelPin,swivlePark*mult);
    delay(100);
    thorax->theSSC.setServo(liftPin,liftPark*mult);
    delay(100);
    thorax->theSSC.setServo(kneePin,kneePark*mult);
    legState = parked;
    delay(100);
  }
}




// ***********************************************
// antThorax code below


antThorax::antThorax(void) {

  legList[0] = new antLeg(this,0,swivel0,lift0,knee0);
  legList[1] = new antLeg(this,1,swivel1,lift1,knee1);
  legList[2] = new antLeg(this,2,swivel2,lift2,knee2); 
  legList[3] = new antLeg(this,3,swivel3,lift3,knee3);
  legList[4] = new antLeg(this,4,swivel4,lift4,knee4);
  legList[5] = new antLeg(this,5,swivel5,lift5,knee5);
  sliceMapper = new mapper(0,SLICES,0,100);
  waitTimer = new timeObj(STEP_SET_WAIT);
  stepState = unknown;
}


void antThorax::startSerial(void) {

  theSSC.initSSC(true);           // Grab the serial port for ourselves.
}


void antThorax::stopSerial(void) {

  Serial.end();
  stepState = unknown;
}


void antThorax::park() {

  int i;

  for (i=0;i<NUM_LEGS;i++) {
    if (legList[i]!=NULL)
      legList[i]->park();
  }
  stepState = parked;
}


void antThorax::fold() {

  int i;

  for (i=0;i<NUM_LEGS;i++) {
    if (legList[i]!=NULL)
      legList[i]->fold();
  }
  stepState = folded;
}


void antThorax::walkFwd() {

  byte i;

  switch (stepState) {
  case walkingFwd :
    for (i=0;i<NUM_LEGS;i++) {
      legList[i]->walkFwd();
    }
    break;
  case startingToWalk :
    legList[0]->walkFwd();
    legList[3]->walkFwd();
    legList[4]->walkFwd();
    if (waitTimer->ding()) {
      legList[1]->walkFwd();
      legList[2]->walkFwd();
      legList[5]->walkFwd();
      stepState = walkingFwd;
    }
    break;
  default :
    waitTimer->start();
    legList[0]->walkFwd();
    legList[3]->walkFwd();
    legList[4]->walkFwd();
    stepState = startingToWalk;
    break;
  }
}


boolean antThorax::checkMapperNum(int mapperNum) {

  return mapperNum>=0 && mapperNum <= NUM_MAPPERS;
}


void antThorax::addMapperPoint(int legMapperNum, double x, double y) {

  if (checkMapperNum(legMapperNum)) {
    legMapper[legMapperNum].addPoint(x,y);
  }
}


double antThorax::mapPoint(int legMapperNum, double x) {

  if (checkMapperNum(legMapperNum)) {
    return legMapper[legMapperNum].Map(x);
  } 
  else
    return 0;
}

// This is where the logic like turning, opposite sides etc. happens. 
// The leg gives us the joint & time, we have the mapper and other 
// information to bundle up into where we want that joint to be set.
// No this bit isn't complete. Only legs 1 & 3 have been built.
double antThorax::legsMapPoint(int legNum, int jointNum, double x) {

  switch(legNum) {
  case 0 :
    switch(jointNum) {
    case swivel :
      return legMapper[swivel].Map(x);
      break;
    case lift :
      return legMapper[lift].Map(x);
      break;
    case knee :
      return legMapper[knee].Map(x);
      break;
    }
    break;
  case 1 :
    switch(jointNum) {
    case swivel :
      return legMapper[swivel].Map(x);
      break;
    case lift :
      return legMapper[lift].Map(x);
      break;
    case knee :
      return legMapper[knee].Map(x);
      break;
    }
    break; 
  case 2 :
    switch(jointNum) {
    case swivel :
      return legMapper[swivel].Map(x);
      break;
    case lift :
      return legMapper[lift].Map(x);
      break;
    case knee :
      return legMapper[knee].Map(x);
      break;
    }
    break; 
  case 3 :
    switch(jointNum) {
    case swivel :
      return legMapper[swivel].Map(x);
      break;
    case lift :
      return legMapper[lift].Map(x);
      break;
    case knee :
      return legMapper[knee].Map(x);
      break;
    }
    break; 
  case 4 :
    switch(jointNum) {
    case swivel :
      return legMapper[swivel].Map(x);
      break;
    case lift :
      return legMapper[lift].Map(x);
      break;
    case knee :
      return legMapper[knee].Map(x);
      break;
    }
    break; 
  case 5 :
    switch(jointNum) {
    case swivel :
      return legMapper[swivel].Map(x);
      break;
    case lift :
      return legMapper[lift].Map(x);
      break;
    case knee :
      return legMapper[knee].Map(x);
      break;
    }
    break; 
  }
}


void antThorax::clearMapper(int legMapperNum) {

  if (checkMapperNum(legMapperNum))
    legMapper[legMapperNum].clearMap();
}



void antThorax::mapperPointsToSerial(int legMapperNum) {

  if (checkMapperNum(legMapperNum))
    legMapper[legMapperNum].outputList();
}



void antThorax::dumpMapperToSerial(int LegMapper) {

  int timeSlice;
  float value;

  timeSlice = 0;
  while (timeSlice<=SLICES) {
    value = legMapper[LegMapper].Map(timeSlice);
    Serial.print(timeSlice);
    Serial.print(", ");
    Serial.println(value);
    timeSlice++;
  }
}


double antThorax::mapTime(double timeSlice) {

  return sliceMapper->Map(timeSlice);
}


