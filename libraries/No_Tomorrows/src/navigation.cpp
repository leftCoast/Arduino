// What we are looking at is the complete navigation application code for No tomorrows.
// Basically this is the NMEA2k processor that runs the screen for the boat and collates
// all the navigation data. It also incorporates a GPS chip attached to Serial1 for
// position.

#include <navigation.h>
#include <navDisp.h>
#include <displayObj.h>
#include <strTools.h>
#include <wlessHdlers.h>


#define NAV_DEVICE_ID		6387				// You get 21 bits. Think serial number.
#define NAV_DEFAULT_ADDR	46					// This initial value will be set using the serial monitor.
#define NAV_DEVICE_CLASS	DEV_CLASS_NAV
#define NAV_DEVICE_FUNCT	DEV_FUNC_GNSS

timeObj     timer(2000);
navDisp		ourNavDisp;
navigation  ourNavApp;

navigation::navigation(void) 
	: NMEA2kBase(NAV_DEVICE_ID,NAV_DEVICE_CLASS,NAV_DEVICE_FUNCT) {
	
	NMEA2KBarometer = new barometerHdlr(llamaBrd);
	bearingVal	= NAN; 
	distanceVal	= NAN;
	airPSI		= NAN;
}
	
	
navigation::~navigation(void) {  }

	
void navigation::setup(void) {
	
	NMEA2kBase::setup();
	ourGPS = new GPSReader;
	ourGPS->begin();
	Serial1.begin(9600);
	delay(250);
	screen = (displayObj*)new adafruit_2050(SCREEN_CS,LC_DC,SCREEN_RST);
	screen->begin();
	ourNavDisp.setup();
}


void navigation::loop(void) {
	
	NMEA2kBase::loop();						// Let our ancestors do their thing.
	if (timer.ding()) {
      ourNavDisp.showPos(&(ourGPS->latLon));
      bearing();
      distance();
      airPSI = NMEA2KBarometer->inHg;
      timer.start();
   }
}


float navigation::bearing(void) {
	
	bearingVal = NAN;
	if (destMark.valid()) {
		if (ourGPS->qualVal!=fixInvalid) {
			bearingVal = (float)ourGPS->latLon.trueBearingTo(&destMark);
			if (bearingVal<0) bearingVal = NAN;
			else if (bearingVal>360)  bearingVal = NAN;
		}
	}
	return bearingVal;
}


float navigation::distance(void) {
	
	distanceVal = NAN;
	if (destMark.valid()) {
		if (ourGPS->qualVal!=fixInvalid) {
			distanceVal = (float)ourGPS->latLon.distanceTo(&destMark);
			if (distanceVal<0) bearingVal = NAN;
		}
	}
	return distanceVal;
}


void navigation::checkAddedComs(int comVal) {

	switch(comVal) {
		case getPos			: doGetPos();		break;
		case getCOG			: doGetCOG();		break;
		case getGPSData	: doGetData();		break;
		case setMarkLat	: doSetLat();		break;
		case setMarklon	: doSetLon();		break;
		case getCourse		: doGetBearing();	break;
		case getDist		: doGetDist();		break;
		default				: printHelp();		break;
	}
}



// Allocate and add the handlers for our different NMEA messages we are going to handle or
// create.
bool navigation::addNMEAHandlers(void) {

	
	
	if (addGPSHandlers(llamaBrd)) {
		if (NMEA2KBarometer) {
			llamaBrd->addMsgHandler(NMEA2KBarometer);
			return true;
		}
	}
	return false;
}


void 	navigation::addCommands(void) {

	NMEA2kBase::addCommands();
	cmdParser.addCmd(getPos,"pos");
	cmdParser.addCmd(getCOG,"cog");
	cmdParser.addCmd(getGPSData,"gpsdata");
	cmdParser.addCmd(setMarkLat,"setlat");
	cmdParser.addCmd(setMarklon,"setlon");
	cmdParser.addCmd(getCourse,"bearing");
	cmdParser.addCmd(getDist,"dist");
}


void navigation::printHelp(void) {

	NMEA2kBase::printHelp();
	Serial.println("                                   Navigation commands.");
	Serial.println("           ----------------------------------------------------------------------");
	Serial.println("Pos         Shows us our current GPS position.");
	Serial.println("COG         Shows us our current course over ground.");
	Serial.println("GPSData     Shows us data about our GPS fix.");
	Serial.println("setLat      Set latitude of mark.");
	Serial.println("setLon      Set longitude of mark.");
	Serial.println("bearing     Get true course from here to mark.");
	Serial.println("dist        Get nautical miles from here to mark.");
}


// This one seems to have had issues overwriting the reused string, while the first one
// was being sent to the host computer. So, I tried doing a local copy. That seems to have
// solved the issue.
void navigation::doGetPos(void) {
	
	char* outStr = NULL;					
	
	Serial.print("Latitude          : ");				// Send to first bit..
	heapStr(&outStr,ourGPS->latLon.showLatStr());	// Save off a local copy of the string.
	Serial.println(outStr);									// Send out the local copy, while..
	Serial.print("Longitude         : ");				// Send out the second label.
	heapStr(&outStr,ourGPS->latLon.showLonStr());	// Save off a copy of the second string.
	Serial.println(outStr);									// Send out the local copy.
	freeStr(&outStr);											// Release the local string memory.
}


void navigation::doGetCOG(void) {
	
	float	COG;
	
	COG =  ourGPS->trueCourse;
	Serial.print("COG True          : ");
	Serial.print(COG,1);
	Serial.println(" Deg.");
	Serial.print("Mag. deviation    : ");
	Serial.print(ourGPS->magVar,3);
	Serial.print(" ");
	if (ourGPS->vEastWest=='E') {
		Serial.println(" East");
	} else {
		Serial.println(" West");
	}
	Serial.print("COG Magnetic      : ");
	Serial.print(COG,1);
	Serial.println(" Deg.");
}


void navigation::doGetData(void) {

	Serial.print("Date              : ");
   Serial.print(ourGPS->month);
   Serial.print("/");
	Serial.print(ourGPS->day);
	Serial.print("/");
	Serial.println(ourGPS->year);
	
	Serial.print("Time              : ");
	Serial.print(ourGPS->hours);
	Serial.print(":");
	Serial.print(ourGPS->min);
	Serial.print(":");
	Serial.print(ourGPS->sec,0);
	Serial.println(" Zulu.");
	
	doGetPos();
	doGetCOG();
	
	Serial.print("Speed over ground : ");
	Serial.print(ourGPS->groudSpeedKnots,1);
	Serial.println(" kn.");
	
	Serial.print("Fix quality       : ");
	switch(ourGPS->qualVal) {
		case fixInvalid	: Serial.println("Fix invalid.");					break;
		case fixByGPS		: Serial.println("Fix by GPS.");						break; 
		case fixByDGPS		: Serial.println("Fix by differentail GPS.");	break;  
	}
	
	Serial.print("Number satellites : ");
	Serial.println(ourGPS->numSatellites);
	
	Serial.print("Altitude          : ");
	Serial.println(ourGPS->altitude,2);
	Serial.print("Geoid height      : ");
   Serial.println(ourGPS->GeoidalHeight);
   Serial.print("Age of data       : ");
   Serial.println(ourGPS->ageOfDGPSData);
   Serial.print("GPS Station ID    : ");
   Serial.println(ourGPS->DGPSStationID);		
				
	Serial.print("GPS mode          : ");
	switch(ourGPS->operationMode) {
		case manual	: Serial.println("Manual mode.");
		case automatic	: Serial.println("Automatic mode.");
	}
	
   Serial.print("Fix type          : ");
	switch(ourGPS->fixType) {
		case noFix	: Serial.println("No fix available.");	break;
		case twoD	: Serial.println("2D fix only.");		break;
		case threeD	: Serial.println("3D Fix.");				break;
	}
	Serial.print("Satellite IDs     :\t");
	for(int i=0;i<11;i++) {
		if (ourGPS->SVID[i]) {
			Serial.print(ourGPS->SVID[i]);
		} else {
			Serial.print("..");
		}
		Serial.print("\t");
	}
	Serial.println();
	
	Serial.print("PDOP              :");Serial.println(ourGPS->PDOP,1);
	Serial.print("HDOP              :");Serial.println(ourGPS->HDOP,1);
	Serial.print("VDOP              :");Serial.println(ourGPS->VDOP,1);
	
	int		numItems;
	satData*	dataNode;
	
	numItems = ourGPS->satInViewList.getCount();
	Serial.println("-------------------------------");
	Serial.print("Satelites in view : ");Serial.println(numItems);
	for (int i=0;i<numItems;i++) {
		dataNode = (satData*)ourGPS->satInViewList.getByIndex(i);
		if (dataNode) {
			Serial.print("Satellite ID      : ");Serial.println(dataNode->PRNNum);
			Serial.print("Elevation         : ");Serial.println(dataNode->elevation);
			Serial.print("Azimuth            : ");Serial.println(dataNode->azimuth);
			Serial.print("Sig / Noise       : ");Serial.println(dataNode->SigToNoise);
			if (i==numItems-1) {
				Serial.println("-------------------------------");
			} else {
				Serial.println(".....");
			}
		}
	}
}


void navigation::doSetLat(void) {

	int			degInt;
	double		degDou;
	double		min;
	globalPos	localPos;
	bool			validPos;
	
	validPos = false;													// Ain't valid yet.
	if (cmdParser.numParams()==2) {								// If we're looking at 2 params..
		degDou = atof(cmdParser.getNextParam());				// Grab the first param and decode it as a double.
		localPos.setLat(degDou);									// Drop it into our local position object.
		localPos.setLatQuad(cmdParser.getNextParam());		// Next param should be text version of quad. Drop it in as well.
		validPos = localPos.valid();								// Check to see if this is a sane latitude value.
	} else if (cmdParser.numParams()==3) {						// Else, if it has 3 parameters.. Different format here.
		degInt = atoi(cmdParser.getNextParam());				// Grab first param and decode as an integer degree value.
		min =  atof(cmdParser.getNextParam());					// Second param decoded as an double minute value.
		localPos.setLatValue(degInt,min);						// Stuff these two, degree & minute values into our local position object.
		localPos.setLatQuad(cmdParser.getNextParam());		// Third param should be text version of our quadrant.
		validPos = localPos.valid();								// Sanity check the position.
	}																		//
	if (validPos) {													// If this passed the sanity text..
		destMark.copyLat(&localPos);								// We basically tell the user everything was ok.
		Serial.print("Latitude was set to : ");				// Bla bla bla.
		Serial.println(destMark.showLatStr());					// Bla.
	} else {																// Else we give them a kick to do better next time.
		Serial.println("We're looking for either, latitude value & quadrant, (N/S kinda' thing).");
		Serial.println("Or, latitude degree value, minute value and then quadrant.");
		Serial.println("I can't make what you typed match any of these.");
	}
}


void navigation::doSetLon(void) {

	int			degInt;
	double		degDou;
	double		min;
	globalPos	localPos;
	bool			validPos;
	
	validPos = false;													// Ain't valid yet.
	if (cmdParser.numParams()==2) {								// If we're looking at 2 params..
		degDou = atof(cmdParser.getNextParam());				// Grab the first param and decode it as a double.
		localPos.setLon(degDou);									// Drop it into our local position object.
		localPos.setLonQuad(cmdParser.getNextParam());		// Next param should be text version of quad. Drop it in as well.
		validPos = localPos.valid();								// Check to see if this is a sane latitude value.
	} else if (cmdParser.numParams()==3) {						// Else, if it has 3 parameters.. Different format here.
		degInt = atoi(cmdParser.getNextParam());				// Grab first param and decode as an integer degree value.
		min =  atof(cmdParser.getNextParam());					// Second param decoded as an double minute value.
		localPos.setLonValue(degInt,min);						// Stuff these two, degree & minute values into our local position object.
		localPos.setLonQuad(cmdParser.getNextParam());		// Third param should be text version of our quadrant.
		validPos = localPos.valid();								// Sanity check the position.
	}																		//
	if (validPos) {													// If this passed the sanity text..
		destMark.copyLon(&localPos);								// We basically tell the user everything was ok.
		Serial.print("Longitude was set to : ");				// Bla bla bla.
		Serial.println(destMark.showLonStr());					// Bla.
	} else {																// Else we give them a kick to do better next time.
		Serial.println("We're looking for, longitude value & quadrant, (E/W kinda' thing).");
		Serial.println("Or, longitude degree value, minute value and then quadrant.");
		Serial.println("I can't make what you typed match any of these.");
	}
}


void navigation::doGetBearing(void) {

	float	bearDegT;
	
	if (destMark.valid()) {
		if (ourGPS->qualVal!=fixInvalid) {
			bearDegT = bearing();
			Serial.print(bearDegT,0);
			Serial.println(" Degrees true.");
		} else {
			Serial.println("We don't have a valid position fix.");
		}
	} else {
		Serial.println("We don't have a marker to aim at.");
	}
}


void navigation::doGetDist(void) {

	float	dist;
	
	if (destMark.valid()) {
		if (ourGPS->qualVal!=fixInvalid) {
			dist = ourGPS->latLon.distanceTo(&destMark);
			Serial.print(dist,1);
			Serial.println(" Nautical miles.");
		} else {
			Serial.println("We don't have a valid position fix.");
		}
	} else {
		Serial.println("We don't have a marker to aim at.");
	}
}



