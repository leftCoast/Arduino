
#include <lilOS.h>
#include "handheldOS.h"
#include "homePanel.h"

#include <breakout.h>
//#include <grenade.h>
#include <sett.h>
#include <rpnCalc.h>
//#include <navTest.h>
//#include <sTerm.h>
#include <sliderPz.h>
//#include <starTrek.h>
//#include "regionTest.h"
//#include <iconEdit.h>
#include <shopList.h>
#include <debug.h>

/*
#define STEPTIME      10  //ms
#define RAMPUP_START  0
#define RAMPUP_END    2000
#define RAMPDN_START  0
#define RAMPDN_END    1000
*/

// *****************************************************
//                     handheldOS
// *****************************************************


// Our global OS.
handheldOS ourOS;


// Create it..
handheldOS::handheldOS(void) {}


// Delete it..
handheldOS::~handheldOS(void) {}


// Possibly we'll need to do something during startup.
bool handheldOS::begin(void) {

  if (lilOS::begin()) {                     // Sets up the global OSPtr.
    setScr(0);                              // Make it so! 0..100 percent 0 being black.
    pinMode(BEEP_PIN, OUTPUT);
    digitalWrite(BEEP_PIN, HIGH);           // Means off.
  } else {
    Serial.println("OS begin() fail.");
    while (true)
      ;
  }
  return true;
}


// Used to create our custom panels..
panel* handheldOS::createPanel(int panelID) {


  switch (panelID) {
    case homeApp: return new homeScreen();
    case calcApp: return new rpnCalc(calcApp);
    case breakoutApp: return new breakout(breakoutApp);
    case settApp: return new sett(settApp);
    case sliderApp: return new sliderPz(sliderApp);
    case shopListApp: return new shopList(shopListApp);
    default: return NULL;
  }
}


void handheldOS::beep() {

  tone(BEEP_PIN, 500, 35);
  digitalWrite(VIBE_PIN,HIGH);
  delay(10);
  digitalWrite(VIBE_PIN,LOW);
}


void handheldOS::launchPanel(void) {

  beep();
  //hideRedraw();
  lilOS::launchPanel();
}


// 0..100 percent 0 being black.
void handheldOS::setScr(bool onOff) { digitalWrite(SCREEN_PIN,onOff); }                                              


// Things we do behind close doors..
void handheldOS::idle(void) { }


  // Calls to be overwritten by user version.

  int handheldOS::getTonePin(void) {
    return BEEP_PIN;
  }

  const char* handheldOS::getSystemFolder() {
    return "/system/";
  }

  const char* handheldOS::getPanelName(int panelID) {

    switch (panelID) {
      case breakoutApp: return "breakout";
      case settApp: return "sett";
      case calcApp: return "rpnCalc";
      case sliderApp: return "slider";
      case shopListApp: return "shopList";
      default: return NULL;
    }
  }
