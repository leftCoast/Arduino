
#include "floraOS.h"
#include "homePanel.h"
#include "controlPanel.h"
#include "loggingPanel.h"
//#include "src/sTerm/sTerm.h"


#define PLANTBOT_BAUD  9600 //115200 //500000


// *****************************************************
//                        floraOS
// *****************************************************


// Our global OS.
floraOS ourOS;


// Create it..
floraOS::floraOS(void) { }


// Delete it..
floraOS::~floraOS(void) { }


// Possibly we'll need to do something during startup.
// cellOS uses this to tweak its color pallette.
int floraOS::begin(void) { 

  ourComPort.begin(PLANTBOT_BAUD);
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, HIGH); //Means off.
  return litlOS::begin();
}

  
// Used to create our custom panels..
panel* floraOS::createPanel(int panelID) {
  
	pauseUpdates x;
	beep();
	switch (panelID) {
		case homeApp		: 
		   ourComPort.disableBG(false);
		   return new homeScreen();
		case controlApp   : 
		   ourComPort.disableBG(false);
		   return new controlPanel();
		/*case calcApp      : return new rpnCalc();*/
		case loggingApp   : 
         ourComPort.disableBG(false);
		   return new loggingPanel();
       /*
		case sTermApp		:
		   ourComPort.disableBG(false);
		   return new sTermPanel();
		case breakoutApp	: 
         ourComPort.disableBG(true);
		   return new breakout();
      */
		default           : return NULL;
	}
}


void floraOS::beep() { tone(BEEP_PIN, 500,35); }


// Just in case we need it, here it is..
void floraOS::idle(void) { }

void floraOS::loop(void) {

  litlOS::loop();           // Let our default actions run.
  ourComPort.updateTime();  // Give our background process a kick.
}

// Globals
