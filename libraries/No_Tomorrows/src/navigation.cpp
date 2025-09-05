// What we are looking at is the complete navigation application code for No tomorrows.
// Basically this is the NMEA2k processor that runs the screen for the boat and collates
// all the navigation data. It also incorporates a GPS chip attached to Serial1 for
// position.

#include <navigation.h>
#include <navDisp.h>
#include <displayObj.h>
#include <strTools.h>
#include <EEPROM.h>


#define NAV_DEVICE_ID		6387				// You get 21 bits. Think serial number.
#define NAV_DEFAULT_ADDR	46					// This initial value will be set using the serial monitor.
#define NAV_DEVICE_CLASS	DEV_CLASS_NAV
#define NAV_DEVICE_FUNCT	DEV_FUNC_GNSS



navDisp		ourNavDisp;
navigation  ourNavApp;



navigation::navigation(void) 
	: NMEA2kBase(NAV_DEVICE_ID,NAV_DEVICE_CLASS,NAV_DEVICE_FUNCT) {
	
	barometer		= new barometerObj(llamaBrd);
	knotMeter 		= new waterSpeedObj(llamaBrd);
	depthSounder	= new waterDepthObj(llamaBrd);
	fuelGauge		= new fluidLevelObj(llamaBrd);
	engHdler			= new engParam(llamaBrd);
	haveMarkLat		= false;
	haveMarkLon		= false;
	EEPROM.get(UTC_DELTA_E_LOC,hoursOffUTC);
	mPole.copyFromEEPROM(MAG_POLE_E_LOC);
	havePoleLat		= true;
	havePoleLon		= true;
	
	timer.setTime(2000);
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
	 while(Serial1.available()) Serial1.read();
}


void navigation::loop(void) {
	
	NMEA2kBase::loop();						// Let our ancestors do their thing.
	if (timer.ding()) {
      ourNavDisp.showPos(&(ourGPS->latLon));
      timer.start();
   }
}


bool navigation::haveMark(void) {

	if (haveMarkLat && haveMarkLon) {
		return destMark.valid();
	}
	return false;
}


bool navigation::haveMPole(void) {

	if (havePoleLat && havePoleLon) {
		return mPole.valid();
	}
	return false;
}


float navigation::bearingMark(void) {
	
	float	bearingVal;
	
	bearingVal = NAN;
	if (haveMark()) {
		if (ourGPS->valid) {
			bearingVal = (float)ourGPS->latLon.trueBearingTo(&destMark);
			if (bearingVal<0) bearingVal = NAN;
			else if (bearingVal>360)  bearingVal = NAN;
		}
	}
	return bearingVal;
}


float navigation::bearingMPole(void) {
	
	float	bearingVal;
	
	bearingVal = NAN;
	if (haveMPole()) {
		if (ourGPS->valid) {
			bearingVal = (float)ourGPS->latLon.trueBearingTo(&mPole);
			if (bearingVal<0) bearingVal = NAN;
			else if (bearingVal>360)  bearingVal = NAN;
		}
	}
	return bearingVal;
}


float navigation::distance(void) {
	
	float	distanceVal;

	distanceVal = NAN;
	if (haveMark()) {
		if (ourGPS->valid) {
			distanceVal = (float)ourGPS->latLon.distanceTo(&destMark);
			if (distanceVal<0) distanceVal = NAN;
		}
	}
	return distanceVal;
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
	cmdParser.addCmd(deltaUTC,"utc");
	cmdParser.addCmd(setNPoleLat,"setplat");
	cmdParser.addCmd(setNPoleLon,"setplon");
	cmdParser.addCmd(getMPPos,"mpole");
	cmdParser.addCmd(getMCorrect,"mcorrect");
	cmdParser.addCmd(spew,"spew");
}


void navigation::checkAddedComs(int comVal) {

	switch(comVal) {
		case getPos			: doGetPos();									break;
		case getCOG			: doGetCOG();									break;
		case getGPSData	: doGetData();									break;
		case setMarkLat	: haveMarkLat = doSetLat(&destMark);	break;
		case setMarklon	: haveMarkLon = doSetLon(&destMark);	break;
		case getCourse		: doGetBearing();								break;
		case getDist		: doGetDist();									break;
		case deltaUTC		: doUTC();										break;
		case setNPoleLat	:
			havePoleLat = doSetLat(&mPole);
			if (haveMPole()) {
				mPole.writeToEEPROM(MAG_POLE_E_LOC);
			}
		break;
		case setNPoleLon	:
			havePoleLon = doSetLon(&mPole);
			if (haveMPole()) {
				mPole.writeToEEPROM(MAG_POLE_E_LOC);
			}
		break;
		case getMPPos		: doMPole();									break;
		case getMCorrect	: doMCorrect();								break;
		case spew			: doSpew();										break;
		default				: printHelp();									break;
	}
}



// Allocate and add the handlers for our different NMEA messages we are going to handle or
// create.
bool navigation::addNMEAHandlers(void) {

	if (addGPSHandlers(llamaBrd)) {
		if (barometer) {
			llamaBrd->addMsgHandler(barometer);
			if (knotMeter) {
				llamaBrd->addMsgHandler(knotMeter);
				if (depthSounder) {
					llamaBrd->addMsgHandler(depthSounder);
					if (fuelGauge) {
						llamaBrd->addMsgHandler(fuelGauge);						
						if (engHdler) {
							llamaBrd->addMsgHandler(engHdler);
							return true;
						}
					}
				}
			}
		}
	}
	return false;
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
	Serial.println("UTC         Get time delta from UTC we are using. Or, if a parameter is added, set it.");
	Serial.println("setPlat     Set latitude of magnetic pole.");
	Serial.println("setPlon     Set longitude of magnetic pole.");
	Serial.println("mPole       Get position of magnetic pole we are using now.");
	Serial.println("mCorrect    Get correction value from true to magnetic course.");
	Serial.println("spew			 Just spew toggles GPSvdata spewing. Adding on or off works too.");
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


bool navigation::doSetLat(globalPos* inPos) {

	int			degInt;
	double		degDou;
	double		min;
	globalPos	localPos;
	bool			validPos;
	
	if (!inPos) return false;										// Sanity, pass in a NULL? Get a fail.
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
		inPos->copyLat(&localPos);									// Write it to our destination mark.
		Serial.print("Latitude was set to : ");				// We basically tell the user everything was ok.
		Serial.println(inPos->showLatStr());					// Show the value.
		return true;													// Tell 'em it worked.
	} else {																// Else we give them a kick to do better next time.
		Serial.println("We're looking for either, latitude value & quadrant, (N/S kinda' thing).");
		Serial.println("Or, latitude degree value, minute value and then quadrant.");
		Serial.println("I can't make what you typed match any of these.");
	}
	return false;
}


bool navigation::doSetLon(globalPos* inPos) {

	int			degInt;
	double		degDou;
	double		min;
	globalPos	localPos;
	bool			validPos;
	
	if (!inPos) return false;										// Sanity, pass in a NULL? Get a fail.
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
		inPos->copyLon(&localPos);									// Write it to our destination mark.
		Serial.print("Longitude was set to : ");				// We basically tell the user everything was ok.
		Serial.println(inPos->showLonStr());					// Show the value.
		return true;													// Success!
	} else {																// Else we give them a kick to do better next time.
		Serial.println("We're looking for, longitude value & quadrant, (E/W kinda' thing).");
		Serial.println("Or, longitude degree value, minute value and then quadrant.");
		Serial.println("I can't make what you typed match any of these.");
	}
	return false;
}


void navigation::doGetBearing(void) {

	float	bearDegT;
	
	if (haveMark()) {
		if (ourGPS->qualVal!=fixInvalid) {
			bearDegT = bearingMark();
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
	
	if (haveMark()) {
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


void navigation::doUTC(void) {
	
	int	UTCOffset;
	
	if (cmdParser.numParams()==0) {								// If we're looking at no params..
		Serial.print("Time offset from UTC : ");
		Serial.println(hoursOffUTC);
	} else if (cmdParser.numParams()==1) {						// If we got a param..
		UTCOffset = atoi(cmdParser.getNextParam());			// Decode it as an integer.
		if (UTCOffset>=-12&&UTCOffset<=12) {					// Sanity check.
			hoursOffUTC = UTCOffset;								// We can use this value.
			EEPROM.put(UTC_DELTA_E_LOC,hoursOffUTC);			// We save this byte in EEPROM for next time.
			Serial.print("Time offset from UTC set to : ");	// Tell 'em
			Serial.println(hoursOffUTC);							//
		}
	} else {
		Serial.println("Looking for either no param. I'll show you the offset.");
		Serial.println("Or one param and I'll set that as offset for you.");
	}
}


void navigation::doMPole(void) {

	if (haveMPole()) {
		Serial.print("Magnetic pole location : ");
		Serial.print(mPole.showLatStr());
		Serial.print("  ");
		Serial.println(mPole.showLonStr());
	} else {
		Serial.println("No vaid magnetic pole available.");
	}
}


void navigation::doMCorrect(void) {

	float			correction;
	
	if (haveMPole()) {
		if (ourGPS->qualVal!=fixInvalid) {
			correction = bearingMPole();
			if (correction>180) {
				correction = 360 - correction;
			} else {
				correction = - correction;
			}
			Serial.print("Our magnetic correction value : ");
			Serial.print(correction,1);
			Serial.println(" Deg.");
			
		} else {
			Serial.println("Sorry, no idea where we are. No GPS fix available.");
		}
	} else {
		Serial.println("Sorry, no magnetic pole location saved.");
	}
}
		
		
void navigation::doSpew(void) {

	if (cmdParser.numParams()==0) {							// If we're looking at no params..
		ourGPS->setSpew(!(ourGPS->spew));					// We toggle spewing.
	} else if (cmdParser.numParams()==1) {					// If we're looking at one param..
		if (!strcmp(cmdParser.getNextParam(),"on")) {
			ourGPS->setSpew(true);
		} else {
			ourGPS->setSpew(false);
		}
	}
	if (!ourGPS->spew) Serial.print("Spewing off.");
}
		
		
		
		
