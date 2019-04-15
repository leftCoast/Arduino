#include "panel.h"
#include "litlOS.h"   // For nextApp


// And it all starts up again..
panel::panel(int panelID,eventSet inEventSet)
  : drawGroup(0,0,PANEL_WIDTH,PANEL_HEIGHT,inEventSet)
  { mPanelID = panelID; }                               // Save what "kind" of panel we are.


// The world as you know it is ending..
panel::~panel(void) {  }


// Whom ever is managing panels can assign IDs to us for
// their nefarious plans. Here's where we hand it back.
int panel::getPanelID(void) { return mPanelID; }


// setup() & loop() panel style.
void panel::setup(void) { }


void panel::loop(void) {  }


// The default here is to not draw ourselves. You can change that.
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
//
// We'll call this sleep() because its much like telling a thread to "sleep." Originally
// I called it delay() and that didn't work, because when called during idle, it became
// endlessly recursive. 
void panel::sleep(unsigned long ms) {

	if (!idling) {						// If we are not IN the idle loop.. (Don't get all reentrant here!)
		mTimer.setTime(ms);			// Set up our timer. We have a timer just for this?
		while(!mTimer.ding()) {		// While waiting for the timer to expire..
			theIdlers.idle();			// We repeatedly call idle() to keep the background stuff running.
		}
	}
}
