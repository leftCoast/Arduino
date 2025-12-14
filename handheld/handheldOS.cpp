
#include <lilOS.h>
#include "handheldOS.h"
#include "homePanel.h"

#include <breakout.h>
#include <grenade.h>
#include <sett.h>
#include <rpnCalc.h>
//#include <navTest.h>
//#include <sTerm.h>
#include <sliderPuzzle.h>
//#include <starTrek.h>
//#include "regionTest.h"
//#include <iconEdit.h>


#define RAMPUP_START  0
#define RAMPUP_END    1000
#define RAMPDN_START  RAMPUP_END
#define RAMPDN_END    RAMPDN_START + 250
#define STEPTIME      5 //ms

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
bool handheldOS::begin(void) { 

   if (lilOS::begin()) {                        // Sets up the global OSPtr.
      pinMode(BEEP_PIN, OUTPUT);
      digitalWrite(BEEP_PIN, HIGH);          // Means off.
      mScreenTimer.setTime(STEPTIME);
      screenMap.addPoint(RAMPUP_START,0);
      screenMap.addPoint(RAMPUP_END,255);
      screenMap.addPoint(RAMPDN_START,255);
      screenMap.addPoint(RAMPDN_END,0);
   } else {
      Serial.println("OS begin() fail.");
      while(true);
   }
   return true;
}

  
// Used to create our custom panels..
panel* handheldOS::createPanel(int panelID) {

   switch (panelID) {
      case homeApp         : return new homeScreen();
      //case fileOpenApp   : return new fileOpen();
      //case fileSaveApp   : return new fileSave();
      case calcApp         : return new rpnCalc(calcApp);
      //case starTrekApp   : return new starTrekPanel(starTrekApp);
      case breakoutApp     : return new breakout(breakoutApp);
      case grenadeApp      : return new grenade(grenadeApp);
      case settApp         : return new sett(settApp);
      //case rgnTestApp    : return new regionTest();
      //case iconEditApp   : return new iconEdit();
      //case navTestApp    : return new navTest(navTestApp);
      //case sTermApp       : return new sTerm(sTermApp);
      case sliderApp      : return new sliderPuzzle(sliderApp);
      default              : return NULL;
   }
}


void handheldOS::beep() { tone(BEEP_PIN, 500,35); }


void handheldOS::launchPanel(void) {
   
   beep();
   hideRedraw();
   lilOS::launchPanel();
}


void handheldOS::hideRedraw() {

  for(int i=RAMPDN_START;i<=RAMPDN_END;i=i+STEPTIME) {
    analogWrite(SCREEN_PIN,screenMap.map(i));
    delay(STEPTIME);
  }
  analogWrite(SCREEN_PIN,0);
  mDimScreen = true;
  mNowTime = RAMPUP_START;
  mEndTime = RAMPUP_END;
  mScreenTimer.start();
}


void handheldOS::bringUp() {
  
  mDimScreen = true;
  mNowTime = RAMPUP_START;
  mEndTime = RAMPUP_END;
  mScreenTimer.start();
}


/// Things we do behind close doors..
void handheldOS::idle(void) { 

  if (mDimScreen) {
    if (mScreenTimer.ding()) {
      analogWrite(SCREEN_PIN,screenMap.map(mNowTime));
      mNowTime = mNowTime + STEPTIME;
      mDimScreen = mNowTime < mEndTime;
    }
    if (!mDimScreen) analogWrite(SCREEN_PIN,255);
  }
}


// Calls to be overwritten by user version.

int handheldOS::getTonePin(void) { return BEEP_PIN; }

const char* handheldOS::getSystemFolder() { return "/system/"; }

const char* handheldOS::getPanelName(int panelID) {

    switch(panelID) {
      case breakoutApp  : return "breakout";
      case grenadeApp   : return "grenade";
      case settApp      : return "sett";
      case calcApp      : return "rpnCalc";
      case sliderApp    : return "slider";
      case navTestApp   : return "navTest";
      case sTermApp     : return "sTerm";
      default           : return NULL;
    }
}
