
#include "litlOS.h" 
#include <screen.h>
#include "rpnCalc.h"


apps      nextApp;  // What panel do we want showing now?
colorObj  aColor;   // A color object anyone can use.


// 
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

void homePanel::loop(void) {  }


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
  nextApp = homeApp;
  hookup();
}


void litlOS::initOSFile(void) {  }


void litlOS::launchPanel(void) {

  if (mPanel) {
    delete mPanel;
    mPanel = NULL;  // Just in case..
  }
  switch(nextApp) {
    case noPanel      : return;
    case homeApp      : mPanel = (panel*) new homePanel(); break;
    case calcApp      : mPanel = (panel*) new rpnCalc();   break;
    case phoneApp     : 
    case textApp      : 
    case contactApp   :
    case qGameApp     :
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
  if (mPanel) {
    mPanel->loop();
  }
 }


// Things we do..
void litlOS::idle(void) {  }
