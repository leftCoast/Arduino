#ifndef eventLog_h
#define eventLog_h

/*

Better talk about what this eventLog is all about before I forget.

The idea behind the event log class is that some calling program wants to log certain
events. Common enough thing to do. From the user point of view, one creates a string
representing the event information then calls addEvent() with this string. addEvent()
timecodes the even and popps it into your SD drive file.

What an event looks like :

Its a text file where each event is formatted :  series# TAB seconds in series TAB user data EOL

Basically tab delimited text, exactly what spreadsheets like to see.

series# : The first time you start logging to a file, its series 0, shut it down, start
again, series 1. Etc.

The time is the seconds that have elapsed while running in the current series. Time units
are seconds, because that fit what I needed to use it for when I wrote it. If you would
like something different, you could probably inherit this and tweak it to suit.

This version is derived from idler to give it a crude clock for timing the seconds. A real
time clock would be a neat addition.

But not today.

As of..
5/18/2020 : Untested. Heck, I don't think its even been compiled.

*/

#include "Arduino.h"
#include "SD.h"
#include "timeObj.h"
#include	"idlers.h"


#define	EVENTLOG_MS	1000	// 1000 gives us a "seconds" field as seconds. Adjust for different units.


class eventLog	:	public idler {

	public:
				eventLog(void);
	virtual	~eventLog(void);
	
	virtual	bool	begin(char* path);
	virtual	int	findSeries(void);
	virtual	void	setLogging(bool offOn);
	virtual	bool	getLogging(void);
	virtual	bool	addEvent(char* eventTxt);
	virtual	void	deleteFile(void);
	virtual	void	idle(void);
				const char*	getPath(void);
	private:
				char*		mPath;
				int		mSeries;
				bool		mReady;
				bool		mLogging;
};

#endif
				
				
	
				