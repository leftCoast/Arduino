
#include <lilOS.h>
#include <resizeBuff.h>
#include <strTools.h>
//#include <debug.h>

int		nextPanel	= NO_PANEL_ID;	// What panel do we want showing now?
lilOS*	OSPtr			= NULL;			// Used by "packages" that need to access the OS's stuff.
panel*	ourPanel		= NULL;
int		panelWith	= 0;
int		panelHeight	= 0;


// *****************************************************
// ********************   appIcon  *********************
// *****************************************************


appIcon::appIcon(int xLoc,int yLoc,int message,const char* path,int pix)
  : iconButton(xLoc,yLoc,path,pix) { mMessage = message;}

  
appIcon::~appIcon(void) { }


void appIcon::doAction(void) { nextPanel = mMessage; }


	
// *****************************************************
// *******************     panel    ********************
// *****************************************************
	
	
// And it all starts up again..
panel::panel(int panelID,menuBarChoices menuBarChoice,eventSet inEventSet)
  : drawGroup(0,0,panelWith,panelHeight,inEventSet) {
  
	mPanelID		= panelID;							// Save what "kind" of panel we are.
	//mOSPtr		= OSPtr;								// Save off our copy to the OS.
	mMenuBar		= NULL;								// Default to NULL.
	mFilePath	= NULL;								// This too.
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
	ourPanel = this;
 }


// The world as you know it, is ending..
panel::~panel(void) { 
	
	resizeBuff(0,&mFilePath);
	ourPanel = NULL;
}


// Whom ever is managing panels can assign IDs to us for
// their nefarious plans. Here's where we hand it back.
int panel::getPanelID(void) { return mPanelID; }



// Given a filename from our folder, generate the fullpath to it.
bool panel::setFilePath(const char* inName) {
	
	char*			folderPtr;
	int			pathLen;
	bool			success;
	

	success = false;
	folderPtr = NULL;
	//if (heapStr(&folderPtr,mOSPtr->getPanelFolder(mPanelID))) {	// If we got a folder path..
	if (heapStr(&folderPtr,OSPtr->getPanelFolder(mPanelID))) {	// If we got a folder path..
		pathLen = strlen(folderPtr);										// Num chars in this path..
		pathLen = pathLen + strlen(inName) + 1;						// Add more for the file name, '\' and '\0'.
		if (resizeBuff(pathLen,&mFilePath)) {							// If we can get the RAM for the path..
			strcpy(mFilePath,folderPtr);									// Our folder path goes in.
			strcat(mFilePath,inName);										// File name goes in.
			success = true;													// Looks good!
		}																			//
		freeStr(&folderPtr);													// Recycle our local copy.
	}																				//
	return success;															// Return our result.
}


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


// We get a close command, we close! At least by default.
void panel::handleCom(stdComs comID) {

	switch(comID) {
		case closeCmd	: close(); break;
		default			: break;
	}
}



// *****************************************************
// *******************   homePanel  ********************
// *****************************************************


homePanel::homePanel(void)
  : panel(HOME_PANEL_ID,noMenuBar) { } // Home panels have no panel to return to.


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

	pathBuff 	= NULL;
	mPanel		= NULL;
	nextPanel	= HOME_PANEL_ID;
}


lilOS::lilOS(int homeID) {

  mPanel		= NULL;
  nextPanel	= homeID;
}


lilOS::~lilOS(void) { }


// A good plan would be to inherit, call this, the do your begin.
bool lilOS::begin(void) {
	
	OSPtr			= this;									// Hookup the global pointer to ourselves.
	panelWith	= getPanelWidth();					// Ask our child how wide their panels are.
	panelHeight	= getPanelHeight();					// Ask our child how high their panels are.
	hookup();												// Want to use idle()? Its ready.
	icon32Mask.readFromBMP(stdIconPath(mask32));	// Read out and setup the standard 32x32 icon mask.
	icon22Mask.readFromBMP(stdIconPath(mask22));	// Read out and setup the standard 22x22 icon mask.
	nextPanel = HOME_PANEL_ID;							// Set to the default home panel.
	return true;											// True means good to go!
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
	
	
	if(!mPanel && nextPanel != NO_PANEL_ID) {		// If have no panel and we want one.
		launchPanel();										// Launch a new panel.
	} else if(nextPanel!=mPanel->getPanelID()) {	// Else, if we just want a change of panels.
		launchPanel();										// Launch the new panel.
	}															//
	if (mPanel) { mPanel->loop(); }					// As always, if there is a panel, let it have some loop time.
}


const char* lilOS::stdIconPath(stdIcons theIcon) {
	
	int	numBytes;
	
	numBytes = strlen(getSystemFolder());				// System folder path
	numBytes = numBytes + strlen(STD_ICON_FLDR);		// Standard icon folder path inside system folder
	numBytes = numBytes + 12;								// Max filename.
	numBytes++;													// '\0'.
	if (resizeBuff(numBytes,&pathBuff)) {
		strcpy(pathBuff,getSystemFolder());
		strcat(pathBuff,STD_ICON_FLDR);
		switch(theIcon) {
			case mask22 	: strcat(pathBuff,"mask22.bmp"); 	break;
			case mask32 	: strcat(pathBuff,"mask32.bmp"); 	break;
			case app32 		: strcat(pathBuff,"app32.bmp"); 		break;
			case check22 	: strcat(pathBuff,"check22.bmp"); 	break;
			case check32 	: strcat(pathBuff,"check32.bmp"); 	break;
			case choice32 	: strcat(pathBuff,"choice32.bmp"); 	break;
			case copy32 	: strcat(pathBuff,"copy32.bmp"); 	break;
			case cross22 	: strcat(pathBuff,"cross22.bmp"); 	break;
			case cross32 	: strcat(pathBuff,"cross32.bmp"); 	break;
			case cut32 		: strcat(pathBuff,"cut32.bmp"); 		break;
			case doc16 		: strcat(pathBuff,"doc16.bmp"); 		break;
			case edit22 	: strcat(pathBuff,"edit22.bmp"); 	break;
			case edit32 	: strcat(pathBuff,"edit32.bmp"); 	break;
			case fNew22		: strcat(pathBuff,"fNew22.bmp"); 	break;
			case fNew32		: strcat(pathBuff,"fNew32.bmp"); 	break;
			case folder16	: strcat(pathBuff,"fldr16.bmp"); 	break;
			case fldrRet16	: strcat(pathBuff,"fldrBk16.bmp"); 	break;
			case fSave22	: strcat(pathBuff,"fSave22.bmp"); 	break;
			case fSave32	: strcat(pathBuff,"fSave32.bmp"); 	break;
			case fOpen22 	: strcat(pathBuff,"fOpen22.bmp"); 	break;
			case fOpen32 	: strcat(pathBuff,"fOpen32.bmp"); 	break;
			case FdrNew22	: strcat(pathBuff,"FdrNew22.bmp"); 	break;
			case FdrNew32	: strcat(pathBuff,"FdrNew32.bmp"); 	break;
			case note32		: strcat(pathBuff,"note32.bmp"); 	break;
			case paste32 	: strcat(pathBuff,"paste32.bmp"); 	break;
			case pref22 	: strcat(pathBuff,"pref22.bmp");		break;
			case pref32 	: strcat(pathBuff,"pref32.bmp"); 	break;
			case SDCard16 	: strcat(pathBuff,"SD16.bmp"); 		break;
			case search22	: strcat(pathBuff,"search22.bmp"); 	break;
			case search32	: strcat(pathBuff,"search32.bmp"); 	break;
			case sort22		: strcat(pathBuff,"sort22.bmp"); 	break;
			case sort32		: strcat(pathBuff,"sort32.bmp"); 	break;
			case trashC22	: strcat(pathBuff,"trashC22.bmp"); 	break;
			case trashC32	: strcat(pathBuff,"trashC32.bmp"); 	break;
			case trashR22	: strcat(pathBuff,"trashR22.bmp"); 	break;
			case trashR32	: strcat(pathBuff,"trashR32.bmp"); 	break;
			case warn32		: strcat(pathBuff,"warn32.bmp"); 	break;
			case x22			: strcat(pathBuff,"x22.bmp"); 		break;
			case x32			: strcat(pathBuff,"x32.bmp"); 		break;
		}
	}
	return pathBuff;
}