// What we are looking at is the complete navigation application code for No tomorrows.
// Basically this is the NMEA2k processor that runs the screen for the boat and collates
// all the navigation data. It also incorporates a GPS chip attached to Serial1 for
// position.

#include <navigation.h>

#define NAV_DEVICE_ID		6387				// You get 21 bits. Think serial number.
#define NAV_DEFAULT_ADDR	46					// This initial value will be set using the serial monitor.
#define NAV_DEVICE_CLASS	DEV_CLASS_NAV
#define NAV_DEVICE_FUNCT	DEV_FUNC_GNSS

navigation::navigation(void) 
	: NMEA2kBase(NAV_DEVICE_ID,NAV_DEVICE_CLASS,NAV_DEVICE_FUNCT)
	{  }
	
	
navigation::~navigation(void) {  }

	
void navigation::setup(void) {
	
	NMEA2kBase::setup();
	
}


void navigation::loop(void) {

	NMEA2kBase::loop();
	
}


// Allocate and add the handlers for our different NMEA messages we are going to handle or
// create.
void navigation::addNMEAHandlers(void) {

}