#include "panel.h"
#include "litlOS.h"

panel::panel(int panelID,blockFile* inFile,unsigned long fBlockID)
  : drawGroup(0,0,PANEL_WIDTH,PANEL_HEIGHT,true),
  fileBuff(inFile,fBlockID) { 

  mPanelID = panelID; // Save what "kind" of panel we are.
}


// The world as you know it is ending..
panel::~panel(void) { Serial.println("base panel being deleted."); }


// Whom ever is managing panels can assign IDs to us for
// their nefarious plans. Here's where we hand it back.
int panel::getPanelID(void) { return mPanelID; }


// setup() & loop() panel style.
void panel::setup(void) { }

void panel::loop(void) {  }
