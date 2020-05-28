#include "eventLog.h"
#include "resizeBuff.h"



timeObj	gTimer(EVENTLOG_MS);	// A global second timer, so that all logfiles will have a single
int		gSec		= 0;			// Time index. Like the click of a movie clapperboard.
bool		gStarted	= false;		// Has anyone called gStartTimer() yet?



// First successful call of begin() calls this. It sets up the globals and starts the
// timer.
void gStartTimer(void) {

	gTimer.start();
	gStarted = true;
}


eventLog::eventLog(void) {

	mPath		= NULL;
	mSeries	= 0;
	mReady	= false;
	mLogging	= false;
}


eventLog::~eventLog(void) { if (mPath) resizeBuff(0,&mPath); }


// Program is up and running.Here's where we see if all this might work. Valid file path.
// Check to see if the SD drive is running. Also fires up the second timer for everyone.
bool eventLog::begin(char* path) {

	File	logFile;
	
	mReady = false;										// Just in case, we ain't ready.
	if (path) {												// Sanity, if they gave us a non-null string;
		if (resizeBuff(strlen(path)+1,&mPath)) {	// If we got the RAM to store the path string..
			strcpy(mPath,path);							// Copy the path.
			logFile = SD.open(mPath, FILE_WRITE);  // Lets try to open/create the logfile.
			if (logFile) {									// If we had success..
				logFile.close();                    // Close the file.
				hookup();									// Hookup to the idler's list.
				if (!gStarted) gStartTimer();			// First one to begin, fires off the initializer function.
        		mReady = true;  							// Flag we're up and running.
        	}             			
		}
	}
	return mReady;											// And tell the world!
}


// So much work for such a small thing..
// Open the file, if we have one, and figure out what the last series number was in it.
// Then return our best guess.
int eventLog::findSeries(void) {

	unsigned long	index;
	unsigned long	lastTab;
	char*				numBuff;
	int				numBytes;
	int				series;
	File				logFile;
	
	lastTab = 0;												// Ain't seen one yet.
	numBuff = NULL;											// We always init addresses to NULL.
	series = 0;													// Unless we find differently.
	if (mReady) {												// If the file stuff has been checked out..
		logFile = SD.open(mPath, FILE_READ);  			// Lets try to open the logfile.
		if (logFile) {											// If we got a logFile..
			index = logFile.size();							// Point at the last byte. (+1)
			if (!index) {										// If we're pointing at byte 0..
				logFile.close();								// Close the logfile.
				return series;									// Empty file, return series = 0.	
			}		
			index--;												// Bump back index. (Starting at last byte now)
			logFile.seek(index);								// Point at the data.
			if (logFile.peek()!='\n') {					// If we didn't find an EOL.. (Should have.)
				index++;											// Put the index back to last byte (+1)
			}
			do {													// Start looping..
				index--;											// Bump back index. (Starting at last non-EOL byte now)
				logFile.seek(index);							// Point at the data.
				if (logFile.peek()=='\t') {				// If we run across a tab..
					lastTab = index;							// We track the last tab we saw.
				}	
			} while(index && logFile.peek()!='\n');	// If not at the start and not pointing at an EOL. (Loop again)
			if (logFile.peek()=='\n') {					// If we found an EOL..
				index++;											// Pop back past it. Went too far.
				logFile.seek(index);							// Point at the data.
			}
			if (lastTab) {										// If we saw a tab..
				numBytes = lastTab - index;				// Calculate the length of the number.
				if (resizeBuff(numBytes+1,&numBuff)) {	// If we can allocate the buffer..
					logFile.read(numBuff,numBytes);		// Grab the number.
					numBuff[numBytes] = '\0';				// Stuff in the terminating null char.
					series = atoi(numBuff);					// Read the number out of the text.
					resizeBuff(0,&numBuff);					// Recycle the text buffer.
				}
			}
			logFile.close();									// Close the logfile.
		}
	}
	return series;												// Pass back what we found as the series number.
}


// If we are ready, we can change logging state. each time we shut down, series increments.
void eventLog::setLogging(bool onOff) {

	if (mReady) {
		if (onOff) {
			mSeries  = findSeries() + 1;
		}
		mLogging = onOff;
	}
}


// If we are ready, we pass back logging state. Otherwise? We pass back false.
bool eventLog::getLogging(void) { 

	if (mReady) {
		return mLogging;
	} else {
		return false;
	}
}


// Add a bit of data. This will start it with a series number then seconds index. All data
// is separated by tabs and it should to readable text. Events ends with an EOL character.
// If you want a header, it would be best in inherit this and add that first if the filesize is 0.
bool eventLog::addEvent(char* eventTxt) {
	
	File	logFile;
	bool	success;
	
	success = false;												// Not been a success yet.
	if (getLogging()) {											// If we -are- logging..
		logFile = SD.open(mPath, FILE_WRITE);  			// Lets try to open/create the logfile.
		if (logFile) {												// If we had success..
			logFile.print(mSeries);logFile.print('\t');	// First we print out the series number.
			logFile.print(gSec);logFile.print('\t');		// Next the number of seconds in this series.
			logFile.println(eventTxt);							// And whatever their data ends up being.
			logFile.close();          							// Close the file. (Always leave files closed!)
			success = true;
		}
	}
	return success;
}
			

// Delete the log file. Good for clearing a logfile. The next added event will re-create
// an empty logfile then add the even to it.	
void eventLog::deleteFile(void) {
	
	File	logFile;
	bool	wasLogging;
	
	if (!mReady) return;								// Sanity, no hardware? Bail.
		wasLogging = mLogging;						// Save off weather we were doing the log thing or not.
  		setLogging(false);							// Shut it down.
		logFile = SD.open(mPath, FILE_WRITE);	// Can we open the file?
  		if (logFile) {									// If we could open the file..
			logFile.close();							// Close it.
			SD.remove(mPath);							// And delete it.
	}
	if (wasLogging) {									// If we were logging..
		setLogging(true);								// Fire it back up.
	}
}
	
		
// Things want to use the filepath. Let 'em have a look.
const char* eventLog::getPath(void) { return mPath; }


// Idle time, everyone checks the timer.
void eventLog::idle(void) {

	if (gTimer.ding()) {		// If the timer has expired..
		gSec++;					// Bump the timer.
		gTimer.stepTime();	// restart the timer.
	}
}