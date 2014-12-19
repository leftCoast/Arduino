#ifndef blinker_h
#define blinker_h

#include "idlers.h"
#include "timeObj.h"

// Some defaults in case the user just doesn't care..
#define defPin 13
#define defOnMs 50
#define defPeriodMs 400

class blinker : public idler, timeObj {
	
public:
	blinker(int inPin=defPin,float inOnMs=defOnMs, float inPeriodMs=defPeriodMs,boolean inInverse=false);
	
	void setInverse(boolean inInverse);  // Toggle ground as oppoed to power.
    void setLight(boolean onOff);
    void setBlink(boolean onOff);		// Start or stop the blinking..
	virtual void idle(void);	        // What to do in our spare time?
	
	void setTimes(float inOnMs, float inPeriodMs);	// Want to change the blink?
	
private:
    boolean  init;
    timeObj* onTimer;
	boolean  running;
	boolean  lightOn;
	int      pin;
    boolean  inverse;
};

#endif
