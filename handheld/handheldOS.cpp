
#include "handheldOS.h"
#include "homePanel.h"
#include "src/breakout/breakout.h"
#include "src/starTrek/starTrek.h"
#include "src/rpnCalc/rpnCalc.h"


// *****************************************************
//                     handheldOS
// *****************************************************


// Our global OS.
handheldOS ourOS;


// Create it..
handheldOS::handheldOS(void) { }


// Delete it..
handheldOS::~handheldOS(void) { }


// Possibly we'll need to do something during startup.
// cellOS uses this to tweak its color pallette.
int handheldOS::begin(void) { 

  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, HIGH); //Means off.
  return litlOS::begin();
}

  
// Used to create our custom panels..
panel* handheldOS::createPanel(int panelID) {

   beep();
   switch (panelID) {
      case homeApp      : return new homeScreen();
      case calcApp      : return new rpnCalc();
      case starTrekApp  : return new starTrekPanel();
      case breakoutApp  : return new breakout();
      default           : return NULL;
   }
}


void handheldOS::beep() { tone(BEEP_PIN, 500,35); }


// Just in case we need it, here it is..
void handheldOS::idle(void) { }


//void handheldOS::loop(void) { }
