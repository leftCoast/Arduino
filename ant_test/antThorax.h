#ifndef antThorax_h
#define antThorax_h

#include "multiMap.h"
#include "miniSSC.h"
#include "timeObj.h"

/*
                                   Front
   0           1
    \        /
     \  () /
       O  O
 2 --- O  O --- 3
       O  O
      /    \
     /       \ 
    4         5 
 
 Group A = 0,3,4
 Group B = 1,2,5
 
 */
#define swivel 0
#define lift   1
#define knee   2

#define swivel0 0
#define lift0   1
#define knee0   2

#define swivel1 3
#define lift1   4
#define knee1   5

#define swivel2 6
#define lift2   7
#define knee2   8

#define swivel3 9
#define lift3   10
#define knee3   11

#define swivel4 12
#define lift4   13
#define knee4   14

#define swivel5 15
#define lift5   16
#define knee5   17

#define swivleFold 0
#define liftFold -1
#define kneeFold -1

#define swivlePark 0
#define liftPark 0
#define kneePark 0

#define NUM_LEGS   6                     // Duh! Its an ANT!
#define NUM_JOINTS 3

#define CYCLE_TIME 3000.0               // Time to complete one step
#define SLICES 20.0                      // How many times to update position per step.
#define STEP_SET_WAIT CYCLE_TIME/2
#define NUM_MAPPERS NUM_JOINTS

enum stepStateType { unknown, parked, folded, startingToWalk, walkingFwd };

class antThorax;

class antLeg {

public:
  antLeg(antThorax* inThorax, byte inLegNum, byte inSwivel, byte inLift, byte inKnee);

  void walkFwd(void);
  void fold(void);
  void park(void);

private :
  antThorax*    thorax;       // Our thorax..
  byte          legNum;
  int           mult;  
  byte          swivelPin;    // Output here..
  byte          liftPin;      // and here..
  byte          kneePin;      // and here..
  int           timeSlice;    // Where are we in our step?
  timeObj*      legTimer;     // How long between updates?
  
  float         swivelVal;    // Calculated values to send out to the servos.
  float         liftVal;
  float         kneeVal;
  stepStateType legState;

};


// **************************************************



class antThorax {

public :
  antThorax(void);

  void startSerial(void);
  void stopSerial(void);
  void park(void);
  void fold(void);
  void walkFwd(void);

  boolean checkMapperNum(int mapperNum);
  void addMapperPoint(int legMapperNum, double x, double y);
  double mapPoint(int legMapperNum, double x);
  double legsMapPoint(int legNum, int jointNum, double x);
  void clearMapper(int LegMapper);
  void mapperPointsToSerial(int LegMapper);
  void dumpMapperToSerial(int LegMapper);
  double mapTime(double timeSlice);                           // Acess the slice mapper..

  stepStateType stepState;
  miniSSC  theSSC;                                            // Used to make servos go..
  
private: 
  antLeg*  legList[NUM_LEGS];                                 // just the leg list
  mapper*  sliceMapper;                                       // Mapps from slice number to data scaled to %.
  multiMap legMapper[NUM_MAPPERS];                            // The joint position mappers.
  timeObj* waitTimer;    // We need to wait before doing something.
};


#endif


