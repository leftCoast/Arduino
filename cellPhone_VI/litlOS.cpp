
#include "litlOS.h" 
#include <screen.h>
#include "rpnCalc.h"


apps      nextApp;  // What panel do we want showing now?
colorObj  aColor;   // A color object anyone can use.



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



homePanel::homePanel(blockFile* inFile,unsigned long rootID)
  : panel(homeApp,inFile,rootID,false) {

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
}


homePanel::~homePanel(void) {

  if (calcIcon) delete calcIcon;
  if (textIcon) delete textIcon;
  if (contactIcon) delete contactIcon;
  if (qGameIcon) delete qGameIcon;
  if (breakoutIcon) delete breakoutIcon;
  if (phoneIcon) delete phoneIcon;
}


void homePanel::panelSetup(void) { 

  if (calcIcon)     { addObj(calcIcon);     calcIcon->begin(); }
  if (textIcon)     { addObj(textIcon);     textIcon->begin(); }
  if (contactIcon)  { addObj(contactIcon);  contactIcon->begin(); }
  if (qGameIcon)    { addObj(qGameIcon);    qGameIcon->begin(); }
  if (breakoutIcon) { addObj(breakoutIcon); breakoutIcon->begin(); }
  if (phoneIcon)    { addObj(phoneIcon);    phoneIcon->begin(); }
  setGroupRefresh();
}

void homePanel::panelLoop(void) {  }


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
}


litlOS::~litlOS(void) {  }


void litlOS::begin(void) {

  mFile = new blockFile(SYS_FILE_PATH);
  if (mFile->isEmpty()) {
    initOSFile();
  }
  nextApp = calcApp;//homeApp;
  hookup();
}


void litlOS::initOSFile(void) {  }


void litlOS::launchPanel(void) {

  if (mPanel) {
    Serial.println("Deleting panel");
    Serial.flush();
    delete mPanel;
    Serial.println("Panel deleted..");
    Serial.flush();
  }
  switch(nextApp) {
    case noPanel      : break;
    case homeApp      : Serial.println("Home"); mPanel = (panel*) new homePanel(mFile,mFile->readRootBlockID()); break;
    case phoneApp     : mPanel = (panel*) new homePanel(mFile,mFile->readRootBlockID()); break;
    case textApp      : mPanel = (panel*) new homePanel(mFile,mFile->readRootBlockID()); break;
    case contactApp   : mPanel = (panel*) new homePanel(mFile,mFile->readRootBlockID()); break;
    case calcApp      : Serial.println("Calculator"); mPanel = (panel*) new rpnCalc(mFile,mFile->readRootBlockID());   break;
    case qGameApp     : mPanel = (panel*) new homePanel(mFile,mFile->readRootBlockID()); break;
    case breakoutApp  : mPanel = (panel*) new homePanel(mFile,mFile->readRootBlockID()); break;
  }
  if (mPanel) {
    mPanel->panelSetup();
    mPanel->hookup();
    viewList.addObj(mPanel);
  }
}


// Tell the current panel its loop time.
void litlOS::doLoop(void) {

  if (mPanel) {
    mPanel->panelLoop();
  }
 }


// Things we do..
void litlOS::idle(void) { 
  
  if(!mPanel && nextApp!=noPanel) {             // If have no panel and we want one.
    launchPanel();                              // Launch a new panel.
  } else if(nextApp!=mPanel->getPanelID()) {    // Else, if we just want a change of panels.
    launchPanel();                              // Launch a new panel.
  }
}
