
#include "litlOS.h" 
#include <screen.h>
#include <cellCommon.h>
#include <cellManager.h>
#include "src/rpnCalc/rpnCalc.h"
#include "src/qGame/qGame.h"
#include "phone.h"
#include "cellListener.h"


#define RAMPUP_START  0
#define RAMPUP_END    1500
#define RAMPDN_START  RAMPUP_END
#define RAMPDN_END    RAMPDN_START + 100

#define STEPTIME  20 //ms


apps        nextApp;  // What panel do we want showing now?
litlOS      ourOS;    // Used to manage the panels.


// *****************************************************
// ********************   appIcon  *********************
// *****************************************************


appIcon::appIcon(int xLoc,int yLoc,apps message,char* path)
  : iconButton(xLoc,yLoc,path) { mMessage = message; }

  
appIcon::~appIcon(void) {  }


void appIcon::doAction(void) { nextApp = mMessage; }


// *****************************************************
// *******************   homePanel  ********************
// *****************************************************

#define HP_ICON_X       3
#define HP_ICON_XSTEP  40
#define HP_ICON_Y     275

#define out     mText->appendText
#define outln   mText->appendText("\n")

homePanel::homePanel(void)
  : panel(homeApp,false) {

  int iconX;
  
  iconX = HP_ICON_X;
  phoneIcon = new appIcon(iconX,HP_ICON_Y,phoneApp,"/system/icons/phone.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  textIcon = new appIcon(iconX,HP_ICON_Y,textApp,"/system/icons/text.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  contactIcon = new appIcon(iconX,HP_ICON_Y,contactApp,"/system/icons/contacts.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  calcIcon = new appIcon(iconX,HP_ICON_Y,calcApp,"/system/icons/calc.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  qGameIcon = new appIcon(iconX,HP_ICON_Y,qGameApp,"/system/icons/qGame.bmp");
  iconX=iconX+HP_ICON_XSTEP;
  breakoutIcon = new appIcon(iconX,HP_ICON_Y,breakoutApp,"/system/icons/breakout.bmp");

  mText = new textView(10,10,220,150);
  mText->setTextColors(&yellow,&blue);
  addObj(mText);
  statusTimer.setTime(1500);
}

// Nothing to do. The icons will be automatically dumped and deleted.
homePanel::~homePanel(void) { }


void homePanel::setup(void) { 

  if (calcIcon)     { addObj(calcIcon);     calcIcon->begin(); }
  if (textIcon)     { addObj(textIcon);     textIcon->begin(); }
  if (contactIcon)  { addObj(contactIcon);  contactIcon->begin(); }
  if (qGameIcon)    { addObj(qGameIcon);    qGameIcon->begin(); }
  if (breakoutIcon) { addObj(breakoutIcon); breakoutIcon->begin(); }
  if (phoneIcon)    { addObj(phoneIcon);    phoneIcon->begin(); }
}

void homePanel::loop(void) { 
  
if (statusTimer.ding()) {
  mText->setText("");
  outln;
  out("           cellStatus");outln;outln;
  out(" Battery V  : ");out(statusReg.batteryVolts);out("mV\n");
  out(" Battery %  : ");out(statusReg.batteryPercent);out("%\n");
  out(" RSSI       : ");out(statusReg.RSSI);outln;
  out(" Net Stat   : ");
  switch (statusReg.networkStat) {
    case 0 : out("Not registered"); break;
    case 1 : out("Reg. (home)"); break;
    case 2 : out("Reg. (searching)"); break;
    case 3 : out("Denied"); break;
    case 4 : out("Unknown"); break;
    case 5 : out("Reg. (roaming)"); break;
    default : out("Undefined"); break;
  }
  outln;  
  out(" Volume     : ");out(statusReg.volume);out("\n");
  out(" CallState  : ");
  switch (statusReg.callStat) {
    case 0  : out("Ready"); break;
    case 1  : out("No Status"); break;
    case 2  : out("Unknown"); break;
    case 3  : out("Ringing In"); break;
    case 4  : out("Ringing Out"); break;
    default : out("Unknown II"); break;
  }
  outln;
  out(" Num SMSs   : ");out(statusReg.numSMSs);out("\n");
  out(" Net Time   : ");out(statusReg.networkTime);out("\n");
  statusTimer.start();
  }
}


void homePanel::drawSelf(void) { 

  screen->fillScreen(&white);
}



// *****************************************************
// *********************   litlOS   ********************
// *****************************************************

 
litlOS::litlOS(void) {

  mFile = NULL;
  mPanel = NULL;
  nextApp = noPanel;
  mDimScreen = false;
  mNowTime = 0;
  mEndTime = 0;
}


litlOS::~litlOS(void) {  }


void litlOS::begin(void) {

  mFile = new blockFile(SYS_FILE_PATH);
  if (mFile->isEmpty()) {
    initOSFile();
  }
  nextApp = homeApp;
  hookup();
  mScreenTimer.setTime(STEPTIME);
  screenMap.addPoint(RAMPUP_START,0);
  screenMap.addPoint(RAMPUP_END,255);
  screenMap.addPoint(RAMPDN_START,255);
  screenMap.addPoint(RAMPDN_END,0);
  ourListener.begin(phoneApp);
  bringUp();
}


void litlOS::initOSFile(void) {  }


void litlOS::launchPanel(void) {

  hideRedraw();
  while(screen->touched()); // Hold 'till their finger is off the screen.
  if (mPanel) {
    delete mPanel;
    mPanel = NULL;  // Just in case..
  }
  switch(nextApp) {
    case noPanel      : return;
    case homeApp      : mPanel = (panel*) new homePanel(); break;
    case calcApp      : mPanel = (panel*) new rpnCalc();   break;
    case qGameApp     : mPanel = (panel*) new qGame();   break;
    case phoneApp     : mPanel = (panel*) new phone();   break;
    case textApp      : 
    case contactApp   :
    case breakoutApp  : 
    default           : 
      nextApp = homeApp;
      mPanel = (panel*) new homePanel();
    break;
  }
  if (mPanel) {
    viewList.addObj(mPanel);
    mPanel->setup();
    mPanel->hookup();  
  }
}


// Tell the current panel its loop time.
void litlOS::loop(void) {

  if(!mPanel && nextApp!=noPanel) {             // If have no panel and we want one.
    launchPanel();                              // Launch a new panel.
  } else if(nextApp!=mPanel->getPanelID()) {    // Else, if we just want a change of panels.
    launchPanel();                              // Launch a new panel.
  }
  if (mPanel) { mPanel->loop(); }
 }


void litlOS::hideRedraw() {

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

void litlOS::bringUp() {
  
  mDimScreen = true;
  mNowTime = RAMPUP_START;
  mEndTime = RAMPUP_END;
  mScreenTimer.start();
}


// Things we do..
void litlOS::idle(void) { 

  if (mDimScreen) {
    if (mScreenTimer.ding()) {
      analogWrite(SCREEN_PIN,screenMap.Map(mNowTime));
      mNowTime = mNowTime + STEPTIME;
      mDimScreen = mNowTime < mEndTime;
    }
    if (!mDimScreen) analogWrite(SCREEN_PIN,255);
  }
}
