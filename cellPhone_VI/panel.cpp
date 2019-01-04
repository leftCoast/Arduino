#include "panel.h"
#include "litlOS.h"   // For nextApp

extern apps nextApp;

// And it all starts up again..
panel::panel(int panelID,bool wantClicks)
  : drawGroup(0,0,PANEL_WIDTH,PANEL_HEIGHT,wantClicks)
  { mPanelID = panelID; }                               // Save what "kind" of panel we are.


// The world as you know it is ending..
panel::~panel(void) {  }


// Whom ever is managing panels can assign IDs to us for
// their nefarious plans. Here's where we hand it back.
int panel::getPanelID(void) { return mPanelID; }


// setup() & loop() panel style.
void panel::setup(void) { }


void panel::loop(void) {  }


// To supress default drawing of one's self.
void panel::drawSelf(void) {  }


// Call this when its time to close the panel
// and go back to the home panel.
void panel::close(void) { nextApp = homeApp; }


// DELAY!! OMG! Here's an idea on how to finally kill this beast.
// Just hold in a loop calling idle() 'til it's done.
void panel::delay(unsigned long ms) {

    mTimer.setTime(ms);
    while(!mTimer.ding()) theIdlers.idle();
}
