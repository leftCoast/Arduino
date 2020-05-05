#include "eventLog.h"
#include "resizeBuff.h"



// First successful call of begin() calls this. It sets up the globals and starts the
// timer.
void gStartTimer(void) {

	gTimer(1000).start();
	gStarted = true;
}



timeObj*	gTimer(1000);			// A global second timer, so that all logfiles will have a single
int		gSec		= 0;			// Time index. Like the click of a movie clapperboard.
bool		gStarted	= false;		// Has anyone called gStartTimer() yet?


eventLog::eventLog(void) {

	mPath		= NULL;
	mSeries	= 0;
	mReady	= false;
	mLogging	= false;
}


eventLog::~eventLog(void) { if (mPath) resizeBuff(0,&mPath); }


// Program is up and running.Here's where we see if all this might work. Valid file path.
// Check to see if the SD drive is running. Also fires up the second timer for everyone.
bool eventLog::begin(char* path,int series) {

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
        		mReady = true;               			// Flag we're up and running.
		}
	}
	return mReady;											// And tell the world!
}


// If we are ready, we can change lokking state.
void eventLog::setLogging(bool offOn) { if (mReady) mLogging = onnOff; }


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
void eventLog::addEvent(char* eventTxt) {
	
	File	logFile;
	
	if (getLogging()) {
		logFile = SD.open(mPath, FILE_WRITE);  			// Lets try to open/create the logfile.
		if (logFile) {												// If we had success..
			logFile.print(mSeries);logFile.print('\t');	// First we print out the series number.
			logFile.print(gSec);logFile.print('\t');		// Next the number of seconds in this series.
			logFile.println(eventTxt);							// And whatever their data ends up being.
		}
		logFile.close();          								// Close the file. (Always leave files closed!)
	}
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
		

// Idle time, everyone checks the timer.
void eventLog::idle(void) {

	if (gStartTimer.ding()) {		// If the timer has expired..
		gSec++;							// Bump the timer.
		gTimer.stepTime();			// restart the timer.
	}
}