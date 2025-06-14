#include <NMEA2kBase.h>

#define LC_MANF		35    // Left coast's manufactur's # (I made it up.) As in J/35.
#define LLAMA_CS     10    // llama board chip select pin.
#define LLAMA_RST    6     // llama board reset pin.
#define LLAMA_INT		2     // llama board inturrupt pin. (currently not used.)


enum baseComs {
	noCommand,
   devList,
   showName,
   lastCom
};


NMEA2kBase::NMEA2kBase(uint32_t inDeviceID,byte inDeviceClass,byte inDeviceFunct) {
	
	deviceID		= inDeviceID;			// These three are just to hold the values
	deviceClass	= inDeviceClass;		//  'till used in setupName().
	deviceFunct	= inDeviceFunct;		//
}


NMEA2kBase::~NMEA2kBase(void) {  }

	
void NMEA2kBase::setup(void) {

	Serial.begin(9600);
	llamaBrd = new llama2000(LLAMA_RST,LLAMA_INT);
	if (!llamaBrd) {
		Serial.println("Can not allocate llama board.");
		while(1);
	}
	setupName();
	addNMEAHandlers();
	if (!llamaBrd->begin(getInitialAddr(),arbitraryConfig,LLAMA_CS)) {
      Serial.println("Starting llama board failed!");
      while (1);
   }
}


void NMEA2kBase::setupName(void) {

	// Our ID stuff.
   llamaBrd->setID(deviceID);				// Device ID. We make these up. You get 21 bits.
   llamaBrd->setManufCode(LC_MANF);		// This would be assigned to you by NMEA people.
   llamaBrd->setECUInst(0);				// First netObj (Electronic control unit.)
   llamaBrd->setFunctInst(0);				// First display.
   llamaBrd->setFunction(deviceFunct);	// What kinda' thing OF our device class are we?
   llamaBrd->setVehSys(deviceClass);	// We class of thing are we?
   llamaBrd->setSystemInst(0);			// We are the first of our device class.
   llamaBrd->setIndGroup(Marine);		// What kind of machine are we ridin' on?
}
 

// Add our default command set. 
void NMEA2kBase::addCommands(void) {

	cmdParser.addCmd(devList,"list");
   cmdParser.addCmd(showName,"seename");
   cmdParser.addCmd(showName,"showname");
   cmdParser.addCmd(showName,"name"); 
 };
 
 
// Method to return the initial address.	
byte NMEA2kBase::getInitialAddr(void) {
 return 0;
}


// Typically, save it as next time's initial address.			
void NMEA2kBase::addressChanged(void) {

}


void NMEA2kBase::loop(void) {

	char	aChar;
	int	comVal;
	
	idle();
	if (Serial.available()) {											// If the user has typed a char..
      aChar = Serial.read();											// Read and save it.
      aChar = tolower(aChar);											// Make it all lower case. Makes sorting it out easier.
      Serial.print(aChar);												// Echo it back to the serial monitor.
      comVal = cmdParser.addChar(aChar);							// Parse this char and see what we get back.
      switch(comVal) {													// If we find a command, we call a hanlder for it.
      	case noCommand	: 									break;	// You know, still parsing.
      	default			: checkAddedComs(comVal);	break;	// Possibly a command we know nothing about
      }
   }
}


void NMEA2kBase::checkAddedComs(int comVal) {  }
	