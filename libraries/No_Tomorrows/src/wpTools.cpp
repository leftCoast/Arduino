#include <wpTools.h>
#include <strTools.h>

#define FORMAT_BYTES	150

//*********************************************
//		wayPoint
//*********************************************

char	formatStr[FORMAT_BYTES];

wayPoint::wayPoint(void)
	:globalPos() {
	
	ourName		= NULL;
}


wayPoint::~wayPoint(void) {

	freeStr(&ourName);
}

				
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
	

char*	wayPoint::formatFileStr(void) {

	sprintf(formatStr,"%s\t%11.6f\t%11.6f",ourName,getLatAsDbl(),getLonAsDbl());
	return(formatStr);
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
	setName(inName);
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
		Serial.println(trace->formatFileStr());
		trace = (wpObj*)trace->getNext();
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


