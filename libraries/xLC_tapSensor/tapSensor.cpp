#include "tapSensor.h"


tapSensor::tapSensor(int sensorPin,float sumTime,int numData) {
  
	mTapSum		= 0;
	mSensorPin	= sensorPin;
	mTapTimer	= new timeObj(sumTime);
	mSmoother	= new runningAvg(numData);
}


tapSensor::~tapSensor(void) {
  
    if (mTapTimer) delete(mTapTimer);
    if (mSmoother) delete(mSmoother);
}
 
 
// Setup the ports etc. 
void tapSensor::begin(void) {
  
  pinMode(mSensorPin, INPUT);			// Setup as input.
  digitalWrite(mSensorPin, HIGH);	// Pullups on!
  hookup();									// Fire up the idle stuff..
  mTapTimer->start();					// Set the timer.
}


// Idle time, time to get to work!
void tapSensor::idle(void) {
    
  if (!digitalRead(mSensorPin)) {		// If we have a tap..
    while (!digitalRead(mSensorPin));	// Hold 'till clear.
    mTapSum++;									// Add the tap to the sum.
  }
  if (mTapTimer->ding()) {					// If its time to add the sum to the running average..
    mSmoother->addData(mTapSum);			// Add the data.
    mTapSum = 0;								// Clear it for the next batch.
    mTapTimer->start();						// Restart the tap timer.
  }
}


// Pass back the current vibration average.
float  tapSensor::getTapVal(void) { return mSmoother->getAvg(); }




