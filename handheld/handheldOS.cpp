
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
    /*
    rampUpMap.setValues(RAMPUP_START,RAMPUP_END,0,100);
    rampDnMap.setValues(RAMPDN_START,RAMPDN_END,100,0);
    scrTimer.setTime(STEPTIME,false);
    scrBvalMapper.setValues(0, 100, 0,255);
    scrMode = scrOff;                       // Screen is off!
    */
    scrBvalMapper.setValues(0, 100, 0,255);
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
  //digitalWrite(VIBE_PIN,HIGH);
  //delay(10);
  //digitalWrite(VIBE_PIN,LOW);
}


void handheldOS::launchPanel(void) {

  beep();
  //hideRedraw();
  lilOS::launchPanel();
}


// 0..100 percent 0 being black.
void handheldOS::setScr(float brightness) {

  int bVal;

  bVal = round(scrBvalMapper.map(brightness));  // Map it to 0..255
  Serial.println(bVal);
  analogWrite(SCREEN_PIN, bVal);                // Set in the value for the screen.
  /*
  switch(scrMode) {                             // Now, check the mode we were in when we got here.
    case scrOff :                               // The screen was off.
      if (bVal==255) {                          // If we got 100%
          scrMode = scrOn;                      // we'll say it's on.
      }                                         //
    break;                                      // Bail.
    case scrOn  :                               // If it was completly off..
      if (bVal==0) {                            // And was set to off..
          scrMode = scrOff;                     // Then it's off.
      }                                         //
    break;                                      // We'll bail.
    case scrRising  :                           // Rising?
      if (bVal==255) {                          // And we hit the top?
          scrMode = scrOn;                      // Flag it as on.
      }                                         //
    break;                                      // We're outta' here.
    case scrSetting :                           // Dimming?
      if (bVal==0) {                            // We hit the bottom?
          scrMode = scrOff;                     // then we'll flag it as off.
      }                                         //
    break;                                      // We're off.
  }                                             // For now..
  */
}                                               // Best we can do.

/*
void handheldOS::hideRedraw() {
  ST
  if (scrMode!=scrOff) {
    mNowTime = 0;
    mEndTime = RAMPDN_END;
    scrMode = scrSetting;
  }
  scrTimer.start();
}


void handheldOS::bringUp() {
  ST
  mNowTime = 0;
  mEndTime = RAMPUP_END;
  scrTimer.start();
  scrMode = scrRising;
}
*/

// Things we do behind close doors..
void handheldOS::idle(void) {
/*
  if (scrTimer.ding()) {
    if (scrMode == scrSetting) {
      setScr(rampDnMap.map(mNowTime));
      mNowTime = mNowTime + STEPTIME;
      if (scrMode == scrOff) {
        Serial.println("Screen's off resetting.");
        scrTimer.reset();
      } else {
        scrTimer.start();
      }
    } else if (scrMode == scrRising) {
      setScr(rampUpMap.map(mNowTime));
      mNowTime = mNowTime + STEPTIME;
      if (scrMode == scrOn) {
       Serial.println("Screen's on resetting.");
        scrTimer.reset();
      } else {
        scrTimer.start();
      }
    }
    if (scrMode == scrOff) {
      if (viewList.listIdle()) {
        bringUp();
      }
    }
  }
  */
}


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
