#include <lilOS.h>
#include <wpTools.h>
#include <strTools.h>
#include <SD.h>

#define BUFF_BYTES	150

//*********************************************
//		wayPoint
//*********************************************

char	strBuff[BUFF_BYTES];

wayPoint::wayPoint(void)
	:globalPos() { ourName = NULL; }


wayPoint::~wayPoint(void) { freeStr(&ourName); }

				
void wayPoint::setName(const char* inName) { heapStr(&ourName,inName); }	


char*	 wayPoint::getName(void) { return ourName; }


bool wayPoint::sameName(wayPoint* inWP) {

	char* ours;
	char* theirs;
	
	ours		= NULL;
	theirs	= NULL;
	
	heapStr(&ours,ourName);
	if (ours) {
		lwrCase(ours);
		heapStr(&theirs,inWP->getName());
		if (theirs) {
			lwrCase(theirs);
			if (!strcmp(ours,theirs)) {
				freeStr(&ours);
				freeStr(&theirs);
				return true;
			}
		}
	}
	freeStr(&ours);
	freeStr(&theirs);
	return false;
}

	
// Create a string [Waypoint name] \t [lat in degrees] \t [lon in degrees] \0
char*	wayPoint::formatFileStr(void) {

	sprintf(strBuff,"%s\t%11.6f\t%11.6f",ourName,getLatAsDbl(),getLonAsDbl());
	return(strBuff);
}


// decode a string [Waypoint name] \t [lat in degrees] \t [lon in degrees] \0 Then use
// this to set our values.
bool	wayPoint::readFileStr(const char* aStr) {

	char*		strCpy;
	char*		token;
	double	lat;
	double	lon;
	
	if (aStr) {										// Sanity, we did get something, right?
		strCpy = NULL;								// We need to setup our own copy. Because?
		heapStr(&strCpy,aStr);					// Using strtok() hashes the original.
		if (strCpy) {								// Got a copy to work with?
			token = strtok(strCpy,"\t");		// First token is the name string.
			setName(token);						// Set it in there.
			token = strtok(NULL,"\t");			// Next is latitude as a signed double.
			lat = atof(token);					// Save the value.
			token = strtok(NULL,"\t");			// lastly is logitude.
			lon = atof(token);					// Save the value.
			setPos(lat,lon);						// Use the values to set our position.
			freeStr(&strCpy);						// Recycle our copy.
			return true;							// return our success.
		}												//
	}													//
	return false;									// Or something went terribly wrong. Send a fail.
}



//*********************************************
//		wpObj
//*********************************************

/*
enum sortOrder {
	
	byName,
	byDistance,
	byBearing
}
*/

		
wpObj::wpObj(wpList* inMgr,const char* inName) 
	: wayPoint(), linkListObj() {
	
	ourMgr = inMgr;
	if (inName) {
		setName(inName);
	}
}
	
	
wpObj::~wpObj(void) {  }



// Are we greater than the obj being passed in? Primary sorting function. We'll return
// true to any error. Why? Because we're real and they are not! So we're greater!				
bool wpObj::isGreaterThan(linkListObj* compObj) {

	wpObj*		compWP;
	globalPos*	ourFix;
	int			strResult;
	double		ourDist;
	double		theirDist;
	double		ourBearing;
	double		theirBearing;
	
	if (compObj && ourMgr) {
		compWP = (wpObj*)compObj;
		ourFix = ourMgr->getFix();
		switch(ourMgr->getSortOrder()) {
			case byName			:
				strResult = strncmp (ourName,compWP->getName(),strlen(ourName));
				if (strResult>0) {
					return true;
				}
				return false;
			case byDistance	:
				ourDist = ourFix->distanceTo(this);
				theirDist = ourFix->distanceTo(compWP);
				return ourDist>theirDist;
			case byBearing		:
				ourBearing = ourFix->trueBearingTo(this);
				theirBearing = ourFix->trueBearingTo(compWP);
				return ourBearing>theirBearing;
			default				: 
				return true;
		}
	}
	return true;
}


// Are we less than the obj being passed in? Primary sorting function. We'll return
// false to any error. Why? Because we're real and they are not! So we're greater! (Not
// less than)
bool wpObj::isLessThan(linkListObj* compObj) {
	
	wpObj*		compWP;
	globalPos*	ourFix;
	int			strResult;
	double		ourDist;
	double		theirDist;
	double		ourBearing;
	double		theirBearing;
	
	if (compObj && ourMgr) {
		compWP = (wpObj*)compObj;
		ourFix = ourMgr->getFix();
		switch(ourMgr->getSortOrder()) {
			case byName			:
				strResult = strncmp (ourName,compWP->getName(),strlen(ourName));
				if (strResult<0) {
					return true;
				}
				return false;
			case byDistance	:
				ourDist = ourFix->distanceTo(this);
				theirDist = ourFix->distanceTo(compWP);
				return ourDist<theirDist;
			case byBearing		:
				ourBearing = ourFix->trueBearingTo(this);
				theirBearing = ourFix->trueBearingTo(compWP);
				return ourBearing<theirBearing;
			default				: 
				return false;
		}
	}
	return false;
}


//*********************************************
//		wpList
//*********************************************


wpList::wpList(globalPos* inFix)
	: linkList() {
	
	ourSortOrder = byName;
	ourFix = inFix;
}
				
				
wpList::~wpList(void) {  }
	

// Create a wpObj copy of this wayPoint. Then add it to our list.	
void wpList::addWP(wayPoint* inWP) {
	
	wpObj* newWPObj;
	
	newWPObj = new wpObj(this,inWP->getName());
	if (newWPObj) {
		newWPObj->copyPos(inWP);
		addToEnd(newWPObj);
	}
}


// Send the list off to the serial port.
void wpList::outputList(void) {

	wpObj*	trace;
	
	Serial.println("*** Waypoint list ***"); 
	trace = (wpObj*)getFirst();
	while(trace) {
		sprintf(strBuff,"%18s\t%11.6f\t%11.6f",trace->getName(),trace->getLatAsDbl(),trace->getLonAsDbl());
		Serial.println(strBuff);
		trace = (wpObj*)trace->getNext();
	}
}


// If this can read a list of waypoints it will dump any waypoints it currently has and
// replaces them with the waypoints in the file. Expects : [name \t	lat \t lon \n]	
void wpList::readList(const char* filePath) {

	File		readFile;
	int		buffIndex;
	char		aChar;
	wpObj*	aWPObj;
	
	if (filePath) {
		readFile = SD.open(filePath,FILE_READ);
		if (readFile) {
			buffIndex = 0;
			dumpList();
			while(readFile.available()) {
				aChar = readFile.read();
				if (aChar=='\n') {
					strBuff[buffIndex] = '\0';
					aWPObj = new wpObj(this);
					if (aWPObj) {
						if (aWPObj->readFileStr(strBuff)) {
							addToEnd(aWPObj);
						}
					}
					buffIndex = 0;
				} else {
					strBuff[buffIndex] = aChar;
					buffIndex++;
				}
			}
			readFile.close();
		}
	}
}
		
		
// Saves each waypoint as [name \t	lat \t lon \n]	
void wpList::saveList(const char* filePath) {
	
	File		saveFile;
	wpObj*	trace;
	
	if (filePath) {	
		if (SD.exists(filePath)) {
			SD.remove(filePath);
		}
		saveFile = SD.open(filePath,FILE_WRITE);
		if (saveFile) {
			trace = (wpObj*)getFirst();
			while(trace) {
				saveFile.println(trace->formatFileStr());
				trace = (wpObj*)trace->getNext();
			}
			saveFile.close();
		}
	}
}

	
wpObj* wpList::findWPByName(const char* inName) {

	wayPoint	aWP;
	wpObj*	trace;
	
	if (inName) {
		aWP.setName(inName);
		trace = (wpObj*)getFirst();
		while(trace) {
			if (trace->sameName(&aWP)) {
				return trace;
			}
			trace = (wpObj*)trace->getNext();
		}
	}
	return NULL;
}

		
void wpList::setSortOrder(sortOrder inOrder) { ourSortOrder = inOrder; }


sortOrder wpList::getSortOrder(void) { return ourSortOrder; }


globalPos* wpList::getFix(void) { return ourFix; }


