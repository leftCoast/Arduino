

#include "RCServoObj.h"



// ******************** servoType ********************


servoType::servoType(float secondsPerDegee,float totalSwingDegrees=180) {

   Vmax = -1; // Gotta' work on this one.
   mDegMapper.setValues(0,totalSwingDegrees,0,100);
}


servoType::~servoType(void) {  }



// ******************** RCServoObj ********************


RCServoObj::RCServoObj(int pinNumber,servoType* inServoType)
   :linklistObj(),
   timeObj() {

   mPin              = pinNumber;
   mNewPos           = -1;
   mLastPos          = -1;
   mServoType        = inServoType;
   mControlledMove   = false;
}


RCServoObj::~RCServoObj(void) {  }


void RCServoObj::move(float newPos) {

   if (mNewPos==-1) {
      pinMode(mPin,OUTPUT);
   }
   mNewPos = newPos;
   mControlledMove = false;
}


void RCServoObj::controlledMove(float newPos,float inVMax,float inAccel)

   if (mNewPos==-1) {
      pinMode(mPin,OUTPUT);
   }
   mNewPos           = newPos;
   mVMax             = inVMax;
   mAccel            = inAccel;
   mControlledMove   = true;
};

   
void RCServoObj::controlledDegreeMove(float newPosDeg,float inVmax,float inAccel);

   if (mNewPos==-1) {
      pinMode(mPin,OUTPUT);
   }
   if (mServoType) {
      mNewPos           = mServoType->mDegMapper.map(newPos);
      mVMax             = mServoType->mVelMapper.map(inVMax);
      mAccel            = inAccel;
      mControlledMove   = true;
   }
}


void RCServoObj::createProfie(void) {

}


void RCServoObj::startPulse(void) {

   
}
            

   
bool RCServoObj::isGreaterThan(linkListObj* compObj);
bool RCServoObj::isLessThan(linkListObj* compObj);
    
           
};


class servoController : public linkList,
                        public squareWave {

   public:
            servoController(void);
   virtual  ~servoController(void);

            void  addServo(servo*  newServo);

   protected:
            void  startServos(void);
            void  sortServos(void);
            void  watchServos(void);
   virtual  void  pulseOn(void);

};

extern servoController servoControl;

#endif
