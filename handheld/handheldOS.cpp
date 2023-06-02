
#include "handheldOS.h"
#include "homePanel.h"
//#include <breakout.h>
#include <starTrek.h>
//#include <rpnCalc.h>
//#include "src/regionTest/regionTest.h"
//#include <iconEdit.h>
//#include "src/fileDBox/fileDBox.h"


#define RAMPUP_START  0
#define RAMPUP_END    1500
#define RAMPDN_START  RAMPUP_END
#define RAMPDN_END    RAMPDN_START + 100
#define STEPTIME      20 //ms

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
  mScreenTimer.setTime(STEPTIME);
  screenMap.addPoint(RAMPUP_START,0);
  screenMap.addPoint(RAMPUP_END,255);
  screenMap.addPoint(RAMPDN_START,255);
  screenMap.addPoint(RAMPDN_END,0);
  return lilOS::begin();
}

  
// Used to create our custom panels..
panel* handheldOS::createPanel(int panelID) {

   beep();
   switch (panelID) {
      case homeApp      : return new homeScreen();
      //case fileOpenApp  : return new fileOpen();
      //case fileSaveApp  : return new fileSave();
      //case calcApp      : return new rpnCalc(&ourOS,calcApp);
      case starTrekApp  : return new starTrekPanel(&ourOS,starTrekApp);
      //case breakoutApp  : return new breakout(&ourOS,breakoutApp);
      //case rgnTestApp   : return new regionTest();
      //case iconEditApp  : return new iconEdit();
      default           : return NULL;
   }
}


void handheldOS::beep() { tone(BEEP_PIN, 500,35); }


void handheldOS::launchPanel(void) {

  //hideRedraw();
  lilOS::launchPanel();
}


void handheldOS::hideRedraw() {

  for(int i=RAMPDN_START;i<RAMPDN_END;i=i+STEPTIME) {
    analogWrite(SCREEN_PIN,screenMap.Map(i));
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

// Calls to be overwritten by used version.
int  handheldOS::getTonePin(void) { return BEEP_PIN; }
void  handheldOS::setBrightness(byte brightness) {  }
char*  handheldOS::getSystemFolder() { return NULL; }
char*  handheldOS::getPanelFolder(int panelID) { return NULL; }
