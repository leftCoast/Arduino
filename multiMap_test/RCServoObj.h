#ifndef RCServoObj_h
#define RCServoObj_h


#include "lists.h"
#include "timeObj.h"
#include "squareWave.h"
#include "baseTrapMove.h"



#define   DEF_SEC_PER_60_DEG .10    //.16 seconds / 60deg. Avarage speed spec. used for RC servos. (Smaller value is faster)
#define   DEF_SWING_DEG       90   // Typical servo rotates 180 degrees lock to lock.

// Formula for maximum servo velocity : Percent swing / milisecond = 60 / (SecPer60 * SwingDeg * 10)
// You don't really need to know this to use the code. Its just here for those that need to go under the hood to fix things.


class servoType {

   public:
            servoType(double secondsPer60Degee=DEF_SEC_PER_60_DEG,double totalSwingDegrees=DEF_SWING_DEG);
            ~servoType(void);

            double   mapVel(double inVel);
            double   mapDeg(double inDeg);
            
            mapper   mVelMapper;
            mapper   mDegMapper;
};


class RCServoObj :   public linkListObj,
                     public timeObj {

   public:
            RCServoObj(int pinNumber,servoType* inServoType=NULL);
   virtual  ~RCServoObj(void);

            void  move(double newPos);
            void  controlledMove(double newPos,double inVmax=100,double inAccel=50);
            void  controlledDegreeMove(double newPosDeg,double inVmax=100,double inAccel=50);
            bool  moving(void);
            
            void  startPulse(void);
            void  endPulse(void);
            void  calcNextPos(void);
            
   protected:
  
   virtual  bool  isGreaterThan(linkListObj* compObj);
    virtual bool  isLessThan(linkListObj* compObj);

            int            mPin;
            servoType*     mServoType;
            bool           mControlledMove;
            double         mNextPos;
            double         mEndPos;
            double         mStartPos;
            double         mLastPos;
            baseTrapMove   mTrapMove;
            unsigned long  mTotalTime;
            bool           mForward;
            
};


class servoController : public linkList,
                        public squareWave {

   public:
            servoController(void);
   virtual  ~servoController(void);

            void  addServo(RCServoObj* newServo);

   protected:
   virtual  void  pulseOn(void);
   virtual  void  pulseOff(void);
            void  startServos(void);
            void  watchServos(void);
            void  addNewServos(void);
            void  calcMoves(void);

            RCServoObj* mIdleServoList;

};

extern servoController servoControl;

#endif
