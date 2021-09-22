
#include <lilOS.h>
#include <screen.h>

int	nextPanel = NO_PANEL_ID;   // What panel do we want showing now?

// *****************************************************
// ********************   appIcon  *********************
// *****************************************************


appIcon::appIcon(int xLoc,int yLoc,int message,char* path,int pix)
  : iconButton(xLoc,yLoc,path,pix) { mMessage = message; }

  
appIcon::~appIcon(void) {  }


void appIcon::doAction(void) { nextPanel = mMessage; }



// *****************************************************
// *******************     panel    ********************
// *****************************************************


// And it all starts up again..
panel::panel(lilOS* ourOS,int panelID,menuBarChoices menuBarChoice,eventSet inEventSet)
  : drawGroup(0,0,PANEL_WIDTH,PANEL_HEIGHT,inEventSet) {
  
  	mOSPtr	= ourOS;									// Save a pointer to our OS object.
	mPanelID = panelID;								// Save what "kind" of panel we are.
	mMenuBar = NULL;									// Default to NULL.
	switch (menuBarChoice) {						// Lets see what kind of bar they wish for?
		case noMenuBar			: break;				// None? Fine, we go now.
		case emptyMenuBar		: 						// Now, panels are created by the O.S. during runtime.
			mMenuBar = new menuBar(this,false);	// Meaning, everything is up and running.
			addObj(mMenuBar);							// So, this call will be fine.
		break;	
		case closeBoxMenuBar	:						// Just like the one above.
			mMenuBar = new menuBar(this);			// Default is to have the close box. (Most do)	
			addObj(mMenuBar);
		break;
	}
}


// The world as you know it, is ending..
panel::~panel(void) { }


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


// *****************************************************
// *******************   homePanel  ********************
// *****************************************************


homePanel::homePanel(lilOS* ourOS)
  : panel(ourOS,HOME_PANEL_ID,noMenuBar) { } // Home panels have no panel to return to.


homePanel::~homePanel(void) {  }


void homePanel::setup(void) {  }


void homePanel::loop(void) {  }


void homePanel::drawSelf(void) {

	screen->fillScreen(&white);
	screen->setCursor(5,25);
	screen->setTextColor(&black,&white);
	screen->drawText("Default home panel");
	}



// *****************************************************
// *********************   lilOS   ********************
// *****************************************************

 
lilOS::lilOS(void) {

  mPanel = NULL;
  nextPanel = HOME_PANEL_ID;
}


lilOS::lilOS(int homeID) {

  mPanel = NULL;
  nextPanel = homeID;
}


lilOS::~lilOS(void) {  }


// A good plan would be to inherit, do your begin
// then at some point during that, call this one.
int lilOS::begin(void) {
	Serial.println("Doing hookup..");
  hookup();							// Want to use idle()? Its ready.
  nextPanel = HOME_PANEL_ID;	// Set to the default home panel.
  return 0;							// 0 means no error right? Or does it mean false, fail?
}


// This is the guy you inherit and use to create your custom panels.
panel* lilOS::createPanel(int panelID) { Serial.println("creating panel?"); return new homePanel(this); }


void lilOS::launchPanel(void) {

	Serial.println("Launching panel..");
	while(screen->touched());							// Hold 'till their finger is off the screen. Its important!
	if (mPanel) {											// If we have a panel now..
		mPanel->closing();								// Give the panel one last chance to get its affairs in order.
		delete mPanel;										// Delete the panel.
		mPanel = NULL;										// Flag it. Just in case..
	}
	if (nextPanel!=NO_PANEL_ID) {						// Someone wants a panel.
		mPanel = createPanel(nextPanel);				// See if they can create it.
		if (mPanel) {										// So, we get a panel in the end?
			viewList.addObj(mPanel);					// Hook it up to the global viewList to allow drawing etc.
			mPanel->setup();								// Give it its setup() call. (Just like back home!)
			mPanel->hookup();								// Hookup idle stuff as well.
		} else {
			nextPanel = HOME_PANEL_ID;					// If we could not create a panel, send 'em home.
		}
	}
}


// Tell the current panel its loop time.
void lilOS::loop(void) {
	
	if(!mPanel && nextPanel!=(unsigned int)mPanel) {	// If have no panel and we want one.
		launchPanel();												// Launch a new panel.
	} else if(nextPanel!=mPanel->getPanelID()) {			// Else, if we just want a change of panels.
		launchPanel();												// Launch the new panel.
	}
	if (mPanel) { mPanel->loop(); }							// As always, if there is a panel, let it have some loop time.
}
