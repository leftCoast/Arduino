#include "servo.h"

static mapper servoMapper(-100, 100, 1, 2);   // +/- 0-100 percent, 1 - 2 ms

servo::servo(int inpin)

:PulseOut(inpin,20)                           // our pin with 20ms period.
{ }


servo::~servo(void) {  }


void servo::setServo(float val) {
   
      float pulse;

      pulse = servoMapper.Map(val);
      setWidth(pulse);
   }

