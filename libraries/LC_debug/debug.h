#ifndef debug_h
#define debug_h

#include "Arduino.h"
#include <idlers.h>
#include <timeObj.h>
#include <strTools.h>
#include <RamMonitor.h>


#define TAB	"  "
#define ST stackTrace stackTracer(__PRETTY_FUNCTION__);

extern	void out(void);
extern	void outln(void);
extern	void out(const char* msg);
extern	void outln(const char* msg);
extern	char* cleanName(const char* inName);

extern	RamMonitor ram;

extern	int		stacklevel;
extern	int32_t	exitRAM;
extern	bool		pop;

class stackTrace {

	public:
				stackTrace(const char* fxName);
	virtual	~stackTrace(void);
	
				char*		fName;
				int32_t	startRAM;	
};



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

//#include <RamMonitor.h>

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