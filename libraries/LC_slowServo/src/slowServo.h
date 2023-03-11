#ifndef slowServo_h
#define slowServo_h

#include <Servo.h>
#include <idlers.h>

// THIS CODE RELIES ON THE STANDARD ARDUINO Servo library.
// IT ALSO RELIES ON THE LC_baseTools library that can be
// installed using the Arduino library manager.


class slowServo : public idler {

  public:
          slowServo(int inPin);
  virtual ~slowServo(void);

          void  begin(void);
          void  setMsPerDeg(int inMs);
          void  setDeg(int inDeg);
  virtual void  idle();

          Servo*  servoPtr;
          timeObj degTimer;
          int     pin;
          int     MsDelay;
          int     currentDeg;
          int     desiredDeg;
};

#endif