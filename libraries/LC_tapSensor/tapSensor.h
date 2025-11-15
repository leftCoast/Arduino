#ifndef tapSensor_h
#define tapSensor_h


#include <idlers.h>
#include <timeObj.h>
#include <runningAvg.h>

/*
This originated from the old shakeNTinkle code written for the kiddie's 8x8 backpack LED
matrix. That version originally was written to run both sensors at the same time. That was
just to specific. This version does one sensor and is much more general purpose.

So..

This is all about sensing vibrations through taps. Adafruit sells two tap sensors, one
more and one less sensitive.
Easy to trigger	https://www.adafruit.com/products/1766
Harder to trigger	https://www.adafruit.com/products/1767

Here's what we do with them. Every time idle() is called, we look to see if we have a
trigger on a sensor pin. Triggering would be a pull to ground. If so, we hold 'till the
trigger event is clear and add one to our accumulator. We continue doing this for the
amount of milliseconds that is given by sumTime.

After sumTime expires, we take the sum and add it a running average object. The amount of 
data values each running average object uses is given by the inputted value numData.

Why in the world do we do all of this?
The sensors only give instant binary readouts. This is good for, say.. Turning something
on and off but that's about it. Also, you need to be watching the input pin when it
happens. By counting the amount of triggers over a given time period. (integrating) We can
get a more analog value representing how much vibration is going on. We run this through
the running averagers to smooth the values through time. Smooth curve in time as opposed
to random glitches.

So, with that being said, here's your input parameters.
int	sensorPin	: pin number for your tap sensor.
float	sumTime		: Amount of time, in milliseconds, to accumulate trigger events. 
int	numData		: Number of data values used in each averager. Carefull here, eats memory with large numbers.
*/


// The default values to get you up and running. You will probably want to overwrite them 
// once you get things running to match your application. Dont't change them here, just
// add the two values to the end of the constructor when you create yours and your values
// will be used instead.

#define DEF_SUMTIME  20  //ms - Can be thought of as sensitivity.
#define DEF_NUMDATA   3  //values per averager. - Can be thought of as capacitance.

// NOTE Multiply the two values gives the approximate time, in ms, for a set of trigger
// events to die down to zero.

class tapSensor : public idler {
  
	public :
				tapSensor(int sensorPin,float sumTime=DEF_SUMTIME,int numData=DEF_NUMDATA);
	virtual	~tapSensor(void);
  
				void	begin(void);
	virtual	void	idle(void);
  				float	getTapVal(void);
  
	protected:
	
		int			mTapSum;
		int			mSensorPin;
		timeObj*		mTapTimer;
		runningAvg*	mSmoother;
};


#endif