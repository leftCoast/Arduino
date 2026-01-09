#ifndef debug_h
#define debug_h

#include "Arduino.h"
#include <idlers.h>
#include <timeObj.h>
#include <strTools.h>



// ******************************************
// ***************   debug    ***************
// ******************************************
		
// Debug class : Prints out a message and optionally stops the process there.


class debug {

  public :
                debug(void);
  virtual       ~debug(void);
  
				void  trace(const char* message,bool hold=false);
				void  trace(const char* message,int inNum,bool hold=false);
				//void  trace(const char* message,float inNum,bool hold=false);
				void  trace(const char* message,const char* inStr,bool hold=false);
};

extern debug db;

#define ST db.trace(__func__,false);




// ******************************************
// *************   traceLoop   **************
// ******************************************


class traceLoop {

	public :
				enum traceStates { idle, active, done };
				
            traceLoop(int inLoops=5);
	virtual	~traceLoop(void);
	
				void	trace(const char* msg);
  				
  				traceStates	ourState;
  				int 			count;
  				int 			loops;
};

extern bool			traceLoopActive;
extern traceLoop	traceList[];



// ******************************************
// ***************  fxTimer   ***************
// ******************************************

// fxTimer; This spits out the amount of time a function takes to execute. Add TIME as the
// first line of any function you would like to time.  And..

/*
void function1(void) {
  TIME  // This makes the magic happen!

  delay(20);
}
*/


class fxTimer {

  public:
    fxTimer(const char* fxName);
    ~fxTimer(void);

  private:
    char*         name;
    unsigned long startMicros;
};




//Handy define to put in your functions..
#define TIME fxTimer time(__func__);



// ******************************************
// *************  RAMMonitor  ***************
// ******************************************


//#define RAM_MONITOR

#ifdef RAM_MONITOR

#include <RamMonitor.h>

class RAMMonitor :	public idler,
							public timeObj {

	public :
				RAMMonitor(float reportMs=500);
  virtual	~RAMMonitor(void);
  				
  				void begin(void);
  virtual	void reportRamStat(const char* aname, uint32_t avalue);
  virtual	void reportRam(void);
  virtual	void idle(void);
  
  				RamMonitor ram;
};

#endif //RAM_MONITOR
	
#endif