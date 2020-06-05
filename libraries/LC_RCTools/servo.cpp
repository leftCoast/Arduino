#include "servo.h"
#include "mapper.h"


static mapper servoMapper(0, 100, 1, 2);   // +/- 0-100 percent, 1 - 2 ms

servo::servo(int inPin)
	:squareWave() { 

	mPin = inPin;
	init = false;
	pinMode(mPin, OUTPUT);
	
}


servo::~servo(void) {  }


void servo::setServo(float val) {
   
	float pulse;

	pulse = servoMapper.Map(val);
	if (!init) {
		pinMode(mPin, OUTPUT);
		setPeriod(18);
		setBlocking(true);
		setOnOff(true);
		init = true;
   }
   setPulse(pulse);
}


void  servo::pulseOn(void) { digitalWrite(mPin, HIGH); }

void  servo::pulseOff(void) { digitalWrite(mPin, LOW); }