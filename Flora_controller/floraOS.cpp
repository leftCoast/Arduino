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

#define BEEP_LOOPS  4
#define BEEP_CONST  500
void floraOS::beep(bool beepUp) {

  unsigned int dTime;
  if (beepUp) {
    for(int i=0;i<BEEP_LOOPS;i++) {
      dTime = BEEP_CONST*(BEEP_LOOPS-i);
      digitalWrite(BEEP_PIN, LOW);
      delayMicroseconds(dTime);
      digitalWrite(BEEP_PIN, HIGH);
      delayMicroseconds(dTime);
    }
  } else {
    for(int i=0;i<BEEP_LOOPS;i++) {
      dTime = BEEP_CONST*BEEP_LOOPS;
      digitalWrite(BEEP_PIN, LOW);
      delayMicroseconds(dTime);
      digitalWrite(BEEP_PIN, HIGH);
      delayMicroseconds(dTime);
    }
  }
}


// Just in case we need it, here it is..
void floraOS::idle(void) { }


// Globals

qCMaster ourComPort;
