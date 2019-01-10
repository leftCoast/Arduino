#include "panel.h"
#include "litlOS.h"   // For nextApp


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


// he default here is to not draw ourselves. You can chnge that.
void panel::drawSelf(void) {  }


// Call this when its time to close the panel. Default is to go back
// to the home panel. Maybe you want to link to another from time to time?
void panel::close(void) { nextPanel = HOME_PANEL_ID; }

// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void panel::closing(void) {  }


// DELAY!! OMG! Here's an idea on how to finally kill this beast.
// Just hold in a loop calling idle() 'til it's done.
void panel::delay(unsigned long ms) {

    mTimer.setTime(ms);
    while(!mTimer.ding()) theIdlers.idle();
}
