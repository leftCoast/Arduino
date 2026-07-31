#include <debug.h>
#include <resizeBuff.h>

int		stacklevel	= 0;
int32_t	exitRAM		= 0;
bool		pop			= true;

RamMonitor ram;

void out(void) { out(""); }

void out(const char* msg) {

	for(int i=0;i<stacklevel;i++) { Serial.print(F(TAB)); }
	Serial.print(msg);
}

void outln(void) { outln(""); }


void outln(const char* msg) {

	out(msg);
	Serial.println();
}


// If a function name starts with "virtual" we'd like to remove that bit. Pretend it
// wasn't there. So, to this end, we'll see if it's there. If so, go beyond the white
// space, then pass back the address to the first printing char after.
char* cleanName(const char* inName) {

	int	i;
	char*	strPtr;
	
	if (inName) {
		strPtr = strstr(inName,"virtual");
		if (strPtr) {
			i = 7;
			while(strPtr[i]) {
				if (!isspace(strPtr[i])) {
					return &(strPtr[i]);
				}
				i++;
			}
		} else {
			return inName;
		}
	}
	return NULL;
}
	
	
stackTrace::stackTrace(const char* fxName) {
	
	fName = NULL;
	startRAM = ram.adj_free();
	for(int i=0;i<stacklevel;i++) { Serial.print(TAB); }
	if (heapStr(&fName,fxName)) {
		Serial.print(cleanName(fName));
		Serial.print(F("\tin  RAM : "));
		Serial.print(startRAM);
		if (pop && exitRAM) {
			if (startRAM!=exitRAM) {
				Serial.print(F("\tDelta : "));
				Serial.print(startRAM - exitRAM);
			}
		}
		Serial.println();
	} else {
		Serial.print(cleanName(fxName));
		Serial.println(F("\tALLOCATION FAIL"));
	}
	pop = false;
	stacklevel++;
}


stackTrace::~stackTrace(void) {
	
	stacklevel--;
	for(int i=0;i<stacklevel;i++) { Serial.print(TAB); }
	if (fName) {
		Serial.print(cleanName(fName));								// Print the function name.
		freeStr(&fName);													// Loose it asap.
		exitRAM = ram.adj_free();
		Serial.print(F("\tout RAM : "));
		Serial.print(exitRAM);
		if (startRAM != exitRAM) {
			Serial.print(F("\tDelta : "));
			Serial.print(exitRAM - startRAM);
		}
		Serial.println();
	} else {
		Serial.println(F("\tALLOCATION FAIL"));
	}
	pop = true;
}


// ******************************************
// ***************   fxTimer  ***************
// ******************************************


fxTimer::fxTimer(const char* fxName) {

  name = NULL;              // Let's not time ourselves..
  heapStr(&name,fxName);    // 
  startMicros = micros();   // So we start here.
}


fxTimer::~fxTimer(void) {

  Serial.print(micros()-startMicros); // Grab time as the first of the last..
  Serial.print(" micros\t");          // And then just close down shop.
  Serial.println(name);               //
  freeStr(&name);                     //
}



// ******************************************
// *************** RAMMonitor ***************
// ******************************************

#ifdef RAM_MONITOR


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

#endif //RAM_MONITOR

