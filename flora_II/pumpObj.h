#ifndef pumpObj_h
#define pumpObj_h

#include "squareWave.h"
#include "multiMap.h"

// First block here is saved in the parameters.
#define DEF_MOTOR_PWM_PERCENT   100     // Basicaly throttle setting.
#define DEF_MOTOR_PWM_PERIOD    20      // PWM peroid. Smooth with control is the goal.


#define MOTOR_1_PIN             16      // Pin the motor runs on. (Hard coded)
#define DELTA_SPEED_TIME        250     // How often do we plan on changing throttle settings, ms. (Also hard coded for now)


class pumpObj : public squareWave {

   public :
            pumpObj(int inPin,float inPWMPercent,float inPWMPeriodMs);
   virtual  ~pumpObj(void);

   virtual  void  setPump(bool onOff);                                                 // Turn the pump on or off. VERY IMPORTANT! TURN OFF PUMP at the start of your program! 
   virtual  bool  pumpOn(void);                                                        // Is the pump on?
   virtual  void  setPulse(float inHighPercent,float inLowPercent,float inPeriodMs);   // This is for setting up speed variance over time.
   virtual  void  setSpeed(void);                                                      // Dynamic speed gets set in here.
   virtual  void  pulseOn(void);                                                       // What to do when pulse goes high.
   virtual  void  pulseOff(void);                                                      // What to do when pulse goes low.

            bool     mInit;
            int      mPin;
            multiMap mSpeedPlot;
            timeObj  mSpeedTimer;
            int      mSpeedFrame;
            int      mPeriod;
};

extern pumpObj ourPump;

#endif
