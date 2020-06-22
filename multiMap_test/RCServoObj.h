#ifndef RCServoObj_h
#define RCServoObj_h

#include "mapper.h"
#include "lists.h"
#include "timeObj.h"
#include "squareWave.h"


class servoType {

   public:
            servoType(float secondsPerDegee,float totalSwingDegrees=180);
            ~servoType(void);

            mapper   mVelMapper;
            mapper   mDegMapper;
};


class RCServoObj :   public linklistObj,
                     public timeObj {

   public:
            RCServoObj(int pinNumber,servoType* inServoType=NULL);
   virtual  ~RCServoObj(void);

            void  move(float newPos);
            void  controlledMove(float newPos,float inVmax=100,float inAccel=50);
            void  controlledDegreeMove(float newPosDeg,float inVmax=100,float inAccel=50);
            
            void  startPulse(void);
            
   protected:
   
   virtual  bool  isGreaterThan(linkListObj* compObj);
    virtual bool  isLessThan(linkListObj* compObj);
    
            bool        mControlledMove;
            float       mNewPos;
            float       mLastPos;
            multiMap    mVProfile;
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
