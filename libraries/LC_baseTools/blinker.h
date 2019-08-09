#ifndef blinker_h
#define blinker_h

#include "idlers.h"
#include "timeObj.h"

// Some defaults in case the user just doesn't care..
#define defPin 13
#define defOnMs 50
#define defPeriodMs 400

class blinker : public idler, public timeObj {
	
public:
				blinker(int inPin=defPin,float inOnMs=defOnMs, float inPeriodMs=defPeriodMs,bool inInverse=false);
	virtual	~blinker(void);
				
				void	setInverse(bool inInverse);	// Toggle ground as oppoed to power.
    			void	setLight(bool onOff);
    			void	setBlink(bool onOff);			// Start or stop the blinking..
	virtual	void	idle(void);							// What to do in our spare time?
	
				void	setTimes(float inOnMs, float inPeriodMs);	// Want to change the blink?
	
	protected:
		bool  	init;
		timeObj*	onTimer;
		bool  	running;
		bool  	lightOn;
		int      pin;
		bool  	inverse;
};

#endif
