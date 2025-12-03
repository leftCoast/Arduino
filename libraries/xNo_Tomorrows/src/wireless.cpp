// What we are looking at is the wireless transmitter application code for No tomorrows.
// Basically this is the NMEA2k processor that gathers the info. and broadcasts it to the
// wrist displays.

#include <wireless.h>
//#include <wlessHdlers.h>

#define WIRELESS_DEVICE_ID			1706				// You get 21 bits. Think serial number.
#define WIRELESS_DEFAULT_ADDR		45					// This initial value will be set using the serial monitor.
#define WIRELESS_DEVICE_SYSTEM	DEV_SYSTEM_DISP
#define WIRELESS_DEVICE_FUNCT		DEV_FUNC_DISP

timeObj     timer(5000);

wireless::wireless(void) 
	: NMEA2kBase(WIRELESS_DEVICE_ID,WIRELESS_DEVICE_SYSTEM,WIRELESS_DEVICE_FUNCT)
	{  }
	
	
wireless::~wireless(void) {  }

	
void wireless::setup(void) {
	
	NMEA2kBase::setup();
	// Do what you need here..
}


void wireless::loop(void) {

	NMEA2kBase::loop();
	// Do what you need here.
	if (timer.ding()) {
      //Show data.
      timer.start();
   }
}


// Allocate and add the handlers for our different NMEA messages we are going to handle or
// create.
bool wireless::addNMEAHandlers(void) {

	return false;
}
