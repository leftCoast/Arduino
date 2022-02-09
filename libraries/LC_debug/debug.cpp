#include "debug.h"
#include <resizeBuff.h>


debug::debug(void) { }
debug::~debug(void) { }

void debug::trace(const char* message,bool hold) {

  Serial.print(message);
  Serial.flush();
  if (hold) {
    while(!Serial.available());
    Serial.read();
  }
  Serial.println(); 
}


void debug::trace(const char* message,const char* inStr,bool hold) {

	Serial.print(message);
	trace(inStr,hold);
}

	
void debug::trace(const char* message,int inNum,bool hold) {

  char* buff;
  int   numChars;
  
  buff = NULL;
  numChars = strlen(message);
  if (resizeBuff(numChars+10,&buff)) {
    strcpy(buff,message);
    snprintf(&(buff[numChars]),9,"%d",inNum);
    trace(buff,hold);
    resizeBuff(0,&buff);
  } else {
    Serial.println(F("NO RAM!!"));
  }
}


// void debug::trace(const char* message,float inNum,bool hold) {
// 
//   char* buff;
//   int   numChars;
//   
//   buff = NULL;
//   numChars = strlen(message);
//   if (resizeBuff(numChars+10,&buff)) {
//     strcpy(buff,message);
//     snprintf(&(buff[numChars]),"9.2%f",inNum);
//     trace(buff,hold);
//     resizeBuff(0,&buff);
//   } else {
//     Serial.println(F("NO RAM!!"));
//   }
// }

debug db;

//****************************************************************************************
//	traceLoop
// Used for things that repeat a LOT. Like loop() or idle(). This gives a global that can
// be switched on in your code and a limit as to how many to print. So as to not flood the
// serial monitor.
//****************************************************************************************


traceLoop::traceLoop(int inLoops) {
	
	loops		= inLoops;
	count		= 0;
	ourState	= idle;
}


traceLoop::~traceLoop(void) {  }


void traceLoop::trace(const char* msg) {  
	
	switch(ourState) {
		case idle	:
			if (traceLoopActive) {
				count = 0;
				db.trace(msg);
				count++;
				if (count>=loops) {
					ourState = done;
				} else {
					ourState = active;
				}
			}
		break;
		case active	:
			if (traceLoopActive) {
				db.trace(msg);
				count++;
				if (count>=loops) {
					ourState = done;
				}
			} else {
				ourState	= idle;
			}
		break;
		case done	:
			if (!traceLoopActive) {
				ourState	= idle;
			}
		break;
	}
}


bool traceLoopActive = false;

traceLoop traceList[5];


//#ifdef RAM_MONITOR

// ******************************************
// *************** RAMMonitor ***************
// ******************************************


RAMMonitor::RAMMonitor(float reportMs)
	: idler(),
	timeObj(reportMs) {  }


RAMMonitor::~RAMMonitor(void) {  }

  
void RAMMonitor::begin(void) {

	ram.initialize();
	hookup();
}


void  RAMMonitor::reportRamStat(const char* aname, uint32_t avalue) {

  Serial.print(aname);
  Serial.print(": ");
  Serial.print((avalue + 512) / 1024);
  Serial.print(" Kb (");
  Serial.print((((float) avalue) / ram.total()) * 100, 1);
  Serial.println("%)");
}


void  RAMMonitor::reportRam(void) {
  
	bool lowmem;
	bool crash;

	Serial.println("==== memory report ====");

	reportRamStat("free", ram.adj_free());
	reportRamStat("stack", ram.stack_total());
	reportRamStat("heap", ram.heap_total());

	lowmem = ram.warning_lowmem();
	crash = ram.warning_crash();
	if(lowmem || crash) {
		Serial.println();
		if(crash)
			Serial.println("**warning: stack and heap crash possible");
		else if(lowmem)
			Serial.println("**warning: unallocated memory running low");
	}
	Serial.println();
}


void RAMMonitor::idle(void) {

	if(ding()) {
		start();
		reportRam();
	}  
	ram.run();
}

//#endif //RAM_MONITOR
