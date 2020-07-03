
#include "RCServoObj.h"

#define STEP_TIME 10    // ms per servo step. dt for all the calculations.


mapper      posMapper(0,100,2,1);      // 0..100% mapps to 1..2ms
mapper      accelMapper(0,100,0,90);   // 0..100% mapps to 0..90deg. Slope of acceleration in degrees.
servoType   defServoType;              // Default servo type. A good choice if you have no special specs. you want to use.


// ******************** servoType ********************


servoType::servoType(double secondsPerDegee,double totalSwingDegrees) {

   double maxVel;
   
   maxVel = 60 / (secondsPerDegee * totalSwingDegrees * 10);
   mVelMapper.setValues(0,100,0,maxVel);
   mDegMapper.setValues(0,totalSwingDegrees,0,100);
}


servoType::~servoType(void) {  }


// Pass in a value from 0 .. 100 percent and get that percentage of the servo's
// max velocity.
double servoType::mapVel(double inVel) { return mVelMapper.map(inVel); }


// Pass in a value from 0 .. the servo's max swing in degrees and get back the the
// 0..100% value to drive the servo.
double servoType::mapDeg(double inDeg) { return mDegMapper.map(inDeg); }



// ******************** RCServoObj ********************


RCServoObj::RCServoObj(int pinNumber,servoType* inType)
   :linkListObj(),
   timeObj() {
   
   mPin              = pinNumber;
   mNextPos          = -1;
   mEndPos           = -1;
   mLastPos          = -1;
   mControlledMove   = false;
   if (inType) {
      mServoType = inType;
   } else {
      mServoType = &defServoType;
   }
}


RCServoObj::~RCServoObj(void) {  }


void RCServoObj::move(double newPos) {

   if (mNextPos==-1) {              // If mNextPos is equal to -1..
      pinMode(mPin,OUTPUT);         // We need to initialze our output pin to be an output pin.
      servoControl.addServo(this);  // And add ourselves to the servo controller.
   }
   mTrapMove.clearMap();            // Keep our memory clear.
   mEndPos           = newPos;      // newPos is given and saved as 0..100% of possible position.
   mNextPos          = newPos;      // Uncontrolled moves go in at one jump.
   mControlledMove   = false;       // This is not going to be a comtroller move. This is fire and forget move.
}


void RCServoObj::controlledMove(double newPos,double inVMax,double inAccel) {

   double   totalDist;
   double   vMax;
   double   rampAngle;
   
   //Serial.print("controlledMove  mEndPos : ");Serial.print(mEndPos);Serial.print("   newPos : ");Serial.print(newPos);Serial.print("   inVMax : ");Serial.print(inVMax);Serial.print("   inAccel : ");Serial.println(inAccel);
   if ( mEndPos == -1 
      || inAccel >= 100 
      || newPos-mLastPos==0 ) {                       // If we have not moved yet, *or* we want 100% acceleration *or* we are asking for 0 distance move..
      move(newPos);                                   // We use a "blind jump" for this first position.
   } else if (inAccel > 0) {                          // Else, we "think" we know where we are *and* if we have a positive acceleration..
      mStartPos         = mLastPos;                   // Save our starting location, because math.
      mEndPos           = newPos;                     // newPos is what we are shooting for as our end position. Positions are given and saved as 0..100% of possible position.
      mNextPos          = mLastPos;                   // Initialize our next position to be where we are now.
      totalDist         = abs(newPos-mLastPos);       // Total distance to travel. Also known as.. Area under velocity curve.
      rampAngle         = accelMapper.map(inAccel);   // User puts in a value of 0..100% accelereation. This maps it to a ramp ange on the velocity curve.
      vMax              = mServoType->mapVel(inVMax); // vMax calculated as the percentge of the possible max velocity.
      mForward          = newPos > mLastPos;          // Moving forward, or not.
      mControlledMove   = true;                       // And this is going to be a controlled move.
      mTrapMove.setupMove(totalDist,vMax,rampAngle);  // Setup the math machine.
      mTotalTime        = 0;                          // Start our internal time keeper.
   }
}


void RCServoObj::controlledDegreeMove(double newPosDeg,double inVmax,double inAccel) {

   double newPos;
   
   newPos = mServoType->mapDeg(newPosDeg);
   controlledMove(newPos,inVmax,inAccel);
}

// Set our timer and turn the pulse to the servo on.
void RCServoObj::startPulse(void) {
   setTime(posMapper.map(mNextPos),true); // Calculate our target position for this pulse, and start timer.
   digitalWrite(mPin,HIGH);               // Flick the pin nto high state.
   mLastPos = mNextPos;                   // Update our last commanded position.
}


// Return if we are doing a controlled moove.
bool RCServoObj::moving(void) { return mControlledMove; }


// Turn the pulse off.
void RCServoObj::endPulse(void) { digitalWrite(mPin,LOW); }


// Calculate our next move position. Update the move time clock.
void RCServoObj::calcNextPos(void) {

   float delta;
   
   if (mControlledMove) {                                                     // If we are doing a controlled move..
      if (mTotalTime>mTrapMove.T) {                                          // If our run time is >= total calulated time..
         mNextPos = mEndPos;                                                  // eliminwte any accumulated error, send to the user commanded position.
         mControlledMove = false;                                             // This controlled move is complete.
      } else {                                                               // Else, we are still working on a controlled move.
         delta = mNextPos;
         if (mForward) {                                                      // If we are moving forward..
            mNextPos = mStartPos + mTrapMove.position(mTotalTime,mForward);   // We calculate our next position
         } else {                                                             // Else, we're moving backwards..
            mNextPos = mStartPos - mTrapMove.position(mTotalTime,mForward);   // We calculate our next position 
         }
         mTotalTime = mTotalTime + STEP_TIME;                                 // Update our internal time keeper.
         //Serial.print(mNextPos-delta);Serial.print("  ");
      }
      //Serial.println(mNextPos);
   }
}


bool RCServoObj::isGreaterThan(linkListObj* compObj) { return mEndPos > ((RCServoObj*)compObj)->mEndPos; }


bool RCServoObj::isLessThan(linkListObj* compObj) { return mEndPos < ((RCServoObj*)compObj)->mEndPos; }
    


// ******************** servoController ********************          

servoController::servoController(void)
   :linkList(),
   squareWave(STEP_TIME,STEP_TIME/2) {
   mIdleServoList = NULL;
}


// If we have some servos hangin on our idle servo list, add them to the managed list so
// that they will be deleted automatically.
servoController::~servoController(void) { addNewServos(); }


// When a servo is commanded to do its first move, it calls this to add itself to the managed servo list.
void servoController::addServo(RCServoObj* newServo) {
   
   newServo->setNext(mIdleServoList);  // Add whatever is in the list to the tail of this new servo.
   mIdleServoList = newServo;          // Point the list at the new servo.
   setOnOff(true);                     // We now have at least one servo to manage. Start up the timers..
}


// BAM! Its time to start all the servo pulses. First start them. Then we have 1 ms to sort them.
// Finally, we sit at the first one and block 'till its ready, move to the next, block, repete. The
// entire process needs to take 3 ms or less to complete.
void servoController::pulseOn(void) {

   if (!isEmpty()) {    // If we have any servos to work with..
      startServos();    // Start all the timed pulses for the servos.
      sort(false);      // Sort the servo list shortest time to longest time.
      watchServos();    // Block 'till we end all the timed pulses.
   }
}


// Halfway mark as the pulse comes off. We add in the idle servo list and give them all a
// kick to calcualte their next positions.
void servoController::pulseOff(void) {
   
   addNewServos();
   calcMoves();
}


// Sweep the idle servo list into our main list of running servos.
void servoController::addNewServos(void) {

   if (mIdleServoList) {         // If we have some servos waiting on the list.
      addToEnd(mIdleServoList);  // We add them to the end of our list of managed servos.
      mIdleServoList = NULL;     // And mark the idle servo list as empty.
   }
}


// As fast as possible, start the on pluse of all the servos.
void servoController::startServos(void) {
 
 RCServoObj* trace;

   trace = (RCServoObj*) getFirst();
   while(trace) {
      trace->startPulse();
      trace = (RCServoObj*)trace->getNext();
   }
}


// All the servos are timing their on pulses. And the list of
// servos has been sorted short to long pulse length. Starting
// at the shortest time length, block each one 'till its ready.
// Then we kick off its pulse and move to the next servo.
void servoController::watchServos(void) {
 
   RCServoObj* trace;

   trace = (RCServoObj*) getFirst();
   while(trace) {
      while(!trace->ding());
      trace->endPulse();
      trace = (RCServoObj*)trace->getNext();
   }
}


// Run down the list of servos and tell them all to calcualte their
// next move positions.
void servoController::calcMoves(void) {
   
   RCServoObj* trace;

   trace = (RCServoObj*) getFirst();
   while(trace) {
      trace->calcNextPos();
      trace = (RCServoObj*)trace->getNext();
   }
}

servoController servoControl;
