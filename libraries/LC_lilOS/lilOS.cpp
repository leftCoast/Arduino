
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
// *******************   homePanel  ********************
// *****************************************************


homePanel::homePanel(void)
  : panel(HOME_PANEL_ID,emptyMenuBar) { } // Home panels have no panel to return to.


homePanel::~homePanel(void) {  }


void homePanel::setup(void) {  }


void homePanel::loop(void) {  }


void homePanel::drawSelf(void) {

	screen->fillScreen(&white);
	screen->setCursor(5,5);
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

  hookup();							// Want to use idle()? Its ready.
  nextPanel = HOME_PANEL_ID;	// Set to the default home panel.
  return 0;							// 0 means no error right? Or does it mean false, fail?
}


// This is the guy you inherit and use to create your custom panels.
panel* lilOS::createPanel(int panelID) { return new homePanel(); }


void lilOS::launchPanel(void) {

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
