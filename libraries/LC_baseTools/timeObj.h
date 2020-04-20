#ifndef timeObj_h
#define timeObj_h

// Insainly handy for doing things in the background.
// Set the timer and wait 'till it goes "ding". 
// Great for blinking LEDs, updating readings, etc.
// Not fast & accurate enough for RC Servos.
// *** Takes care of roll over issues ***

enum timeType { zero, crossing, normal };

class timeObj {

	public:
  				timeObj(float inMs=10);
	virtual	~timeObj(void);
	
  				void 				setTime(float inMs,bool startNow=true);	// Change the time duration for next start..
  	virtual	void 				start(void);                       			// Start the timer "now".
  	virtual	void 				stepTime(void);                    			// Restart the timer from last end time.
  				bool				ding(void);											// Timer has expired.
  				unsigned long	getTime(void);										// How long does this go for?
  				float				getFraction(void);								// Fuel gauge. What fraction of time is left.
    
protected:
  				unsigned long	waitTime;
  				unsigned long	startTime;
  				unsigned long	endTime;
  				timeType			config;
};

#endif

