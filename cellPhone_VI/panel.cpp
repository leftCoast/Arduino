#include "panel.h"
#include "litlOS.h"

panel::panel(blockFile* inFile,unsigned long blockID,bool menu)
  : drawGroup(0,0,PANEL_WIDTH,PANEL_HEIGHT,true),
  fileBuff(inFile,blockID) { 
  
  if (menu) {         // One or two panels user & menu or just user.                               
    mMenuPanel = new drawGroup(0,0,PANEL_WIDTH,SYS_MENU_PIX,true);
    mUserPanel =  new drawGroup(0,SYS_MENU_PIX,PANEL_WIDTH,PANEL_HEIGHT-SYS_MENU_PIX,true);
  } else {
    mMenuPanel = NULL;
    mUserPanel = new drawGroup(0,0,PANEL_WIDTH,PANEL_HEIGHT,true);
  }
  if (mMenuPanel) drawGroup::addObj(mMenuPanel);
  if (mUserPanel) drawGroup::addObj(mUserPanel);
}


// The world as you know it is ending..
panel::~panel(void) {

  if (mUserPanel) delete(mUserPanel);
  if (mMenuPanel) delete(mMenuPanel);
}


// Add objects to the menu or the user panels.
void panel::addObj(drawObj* newObj,bool menu) {

  if (menu) {
    if (mMenuPanel) {
      mMenuPanel->addObj(newObj);
    }
  } else if (mUserPanel) {
    mUserPanel->addObj(newObj);
  }
}


// We'll now default to adding to the user panel.
void panel::addObj(drawObj* newObj) {

  if (mUserPanel) {
    mUserPanel->addObj(newObj);
  }
}


// We shouldn't do any drawing. Our job is book keeping.
// Drawing here would just slow stuff down.
void panel::drawSelf(void) { }


// setup() & loop() panel style.
void panel::panelSetup(void) {  }


void panel::panelLoop(void) {  }
