#include "floraOS.h"
#include "homePanel.h"
#include "controlPanel.h"
#include "loggingPanel.h"


// *****************************************************
// ********************   floraOS   ********************
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

  ourComPort.begin(9600);
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, HIGH); //Means off.
  return litlOS::begin();
}

  
// Used to create our custom panels..
panel* floraOS::createPanel(int panelID) {

  switch (panelID) {
    case homeApp      : return new homeScreen();
    case controlApp   : return new controlPanel();
    case calcApp      : return new rpnCalc();
    case loggingApp   : return new loggingPanel();
    default           : return NULL;
  }
}


void floraOS::beep(bool beepUp) {

	if (!beepUp) {
		tone(BEEP_PIN, 500,35);
	}
}


// Just in case we need it, here it is..
void floraOS::idle(void) { }

void floraOS::loop(void) {

  litlOS::loop();           // Let our default actions run.
  ourComPort.updateTime();  // Give our background process a kick.
}

// Globals
