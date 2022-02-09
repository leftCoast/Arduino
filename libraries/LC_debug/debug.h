#ifndef debug_h
#define debug_h

#include "Arduino.h"
#include <RamMonitor.h>
#include <idlers.h>
#include <timeObj.h>

		

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

//#define RAM_MONITOR

//#ifdef RAM_MONITOR

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

//#endif //RAM_MONITOR
	
#endif