#include "servo.h"

#define steerRight 100
#define steerLeft  -100
#define steerStraight 0

class chassisObj {

public:

   chassisObj(
   
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
   float speedTime,
   float speedGain,
   float inMinPower
   
   );

   void setSteer(float steerVal); // Set steering from steerLeft to steerRight -100..100 percent
   void setPower(float powerVal); // Set a power setting -100..100 percent
   void motorOff(void);           // stop sending updates to the motor servo, (stops it)
   void setRPM(float rpmVal);     // Set a RPM value.
   float readRPM(void);           // how fast are we spinning?
   void setDistance(long counts); // Set a value to the distance counter.
   long getDistance(void);        // Read the distance counter.
   void idle(void);               // To be called in all loops.

private :

   servo*   steer;                // Steering servo
   servo*   motor;                // Motor servo..
   timeObj* speedCheck;          // Clock for timing when to look at speedo.
   bool     motorEnable;          // We want motor running right?
   int      motorMode;            // Power or speed?
   int      speedPin;             // Pin used for inturupt handler.
   int      countsPerRev;          // How many inturrupts do we get per revolution?
   float    rpmVal;               // How fast we want to spin..
   bool     reverse;              // We want backwards?
};

