#include "floraOS.h"
#include "controlPanel.h"

// *****************************************************
// ******************   homeScreen   *******************
// *****************************************************


homeScreen::homeScreen(void) { }

homeScreen::~homeScreen(void) { }
  
void homeScreen::setup(void) { }

void homeScreen::loop(void) { }

void homeScreen::drawSelf(void) {
  
  nextPanel = controlApp;
}



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
    default           : return NULL;
  }
}


void floraOS::beep(void) {

  for(int i=0;i<20;i++) {
    digitalWrite(BEEP_PIN, HIGH);
    delay(1);
    digitalWrite(BEEP_PIN, LOW);
    delay(1);
  }
  digitalWrite(BEEP_PIN, HIGH);
}


// Just in case we need it, here it is..
void floraOS::idle(void) { }


// Globals

qCMaster ourComPort;
