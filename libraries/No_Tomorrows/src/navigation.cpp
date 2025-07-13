// What we are looking at is the complete navigation application code for No tomorrows.
// Basically this is the NMEA2k processor that runs the screen for the boat and collates
// all the navigation data. It also incorporates a GPS chip attached to Serial1 for
// position.

#include <navigation.h>

#define NAV_DEVICE_ID		6387				// You get 21 bits. Think serial number.
#define NAV_DEFAULT_ADDR	46					// This initial value will be set using the serial monitor.
#define NAV_DEVICE_CLASS	DEV_CLASS_NAV
#define NAV_DEVICE_FUNCT	DEV_FUNC_GNSS

timeObj     timer(5000);

navigation::navigation(void) 
	: NMEA2kBase(NAV_DEVICE_ID,NAV_DEVICE_CLASS,NAV_DEVICE_FUNCT)
	{  }
	
	
navigation::~navigation(void) {  }

	
void navigation::setup(void) {
	
	NMEA2kBase::setup();
	ourGPS.begin();
	Serial1.begin(9600);
}


void navigation::loop(void) {
	
	NMEA2kBase::loop();						// Let our ancestors do their thing.
	if (timer.ding()) {
      fixData.showData();
      timer.start();
   }
}


void navigation::checkAddedComs(int comVal) {

	switch(comVal) {
		case getPos			: doGetPos();	break;
		case getCOG			: doGetCOG();	break;
		case getGPSData	: doGetData();	break;
		default				: printHelp();	break;
	}
}



// Allocate and add the handlers for our different NMEA messages we are going to handle or
// create.
bool navigation::addNMEAHandlers(void) { return addGPSHandlers(llamaBrd); }


void 	navigation::addCommands(void) {

	NMEA2kBase::addCommands();
	cmdParser.addCmd(getPos,"pos");
	cmdParser.addCmd(getCOG,"cog");
	cmdParser.addCmd(getGPSData,"gpsdata");
}


void navigation::printHelp(void) {

	NMEA2kBase::printHelp();
	Serial.println("                                   Navigation commands.");
	Serial.println("           ----------------------------------------------------------------------");
	Serial.println("Pos Shows us our current GPS position.");
	Serial.println("COG Shows us our current course over ground.");
	Serial.println("GPSData  Shows us data about our GPS fix.");	
}


void navigation::doGetPos(void) {

	Serial.println(fixData.latLon.showLatStr());
	Serial.println(fixData.latLon.showLonStr());
}


void navigation::doGetCOG(void) {
	
	float	COG;
	
	COG =  trackMadeGood.trueCourse;
	Serial.print("Course over ground : ");
	Serial.print(COG,1);
	Serial.println(" Deg T");
	Serial.print("Magnetic deviation : ");
	Serial.println(minTransData.magVar,1);
	COG =  trackMadeGood.magCourse;
	Serial.print("Course over ground : ");
	Serial.print(COG,1);
	Serial.println(" Deg M");
}


void navigation::doGetData(void) {

	Serial.print("Time              : ");
	Serial.print(fixData.hours);
	Serial.print(":");
	Serial.print(fixData.min);
	Serial.print(":");
	Serial.print(fixData.sec,0);
	Serial.println(" Zulu.");

	
	Serial.print("Fix quality       : ");
	switch(fixData.qualVal) {
		case fixInvalid	: Serial.println("Fix invalid.");					break;
		case fixByGPS		: Serial.println("Fix by GPS.");						break; 
		case fixByDGPS		: Serial.println("Fix by differentail GPS.");	break;  
	}
	
	Serial.print("Number satellites : ");
	Serial.println(fixData.numSatellites);
	
	Serial.print("Altitude          : ");
	Serial.println(fixData.altitude,2);
	Serial.print("Geoid height      : ");
   Serial.println(fixData.GeoidalHeight);
   Serial.print("Age of data       : ");
   Serial.println(fixData.ageOfDGPSData);
   Serial.print("GPS Station ID    : ");
   Serial.println(fixData.DGPSStationID);		
				
	Serial.print("GPS mode          : ");
	switch(activeSatellites.operationMode) {
		case manual	: Serial.println("Manual mode.");
		case automatic	: Serial.println("Automatic mode.");
	}
	
   Serial.print("Fix type          : ");
	switch(activeSatellites.fixType) {
		case noFix	: Serial.println("No fix available.");	break;
		case twoD	: Serial.println("2D fix only.");		break;
		case threeD	: Serial.println("3D Fix.");				break;
	}
	Serial.print("Satellite IDs     :\t");
	for(int i=0;i<11;i++) {
		if (activeSatellites.SVID[i]) {
			Serial.print(activeSatellites.SVID[i]);
		} else {
			Serial.print("..");
		}
		Serial.print("\t");
	}
	Serial.println();
	
	Serial.print("PDOP              :");Serial.println(activeSatellites.PDOP,1);
	Serial.print("HDOP              :");Serial.println(activeSatellites.HDOP,1);
	Serial.print("VDOP              :");Serial.println(activeSatellites.VDOP,1);
}


void navigation::doSetMark(void) {
	
}


void navigation::getBearing(void) {

}


void navigation::getDist(void) {

}



