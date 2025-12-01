#include <fileViewer.h>
#include <resizeBuff.h>
#include <stdComs.h>

//#include <debug.h>

#define DBL_CLICK_MS	500

#define DEF_SELECT_X	30
#define DEF_SELECT_Y	40
#define DEF_SELECT_W	120
#define DEF_SELECT_H	190

// The 16x16 icon stuff.
#define DEF_LIST_ICON_X	2
#define DEF_LIST_ICON_Y	1
#define DEF_LIST_ICON_W	16
#define DEF_LIST_ICON_H	16

// File list, size & num items.
#define DEF_FILE_LIST_X			10
#define DEF_FILE_LIST_Y			50
#define DEF_FILE_LIST_W			100
#define DEF_NUM_LIST_ITEMS		5

// File list item stuff. Where things are placed.
#define DEF_LIST_ITEM_W			DEF_FILE_LIST_W - 2
#define DEF_LIST_ITEM_H			18
#define DEF_LIST_ITEM_TXT_X	22
#define DEF_LIST_ITEM_TXT_Y	6
#define DEF_LIST_ITEM_TXT_W	DEF_FILE_LIST_W - DEF_LIST_ITEM_TXT_X - 2
#define DEF_LIST_ITEM_TXT_H	8

// File list height is a calculation. So it ends up here.
#define FILE_LIST_HEIGHT	DEF_LIST_ITEM_H * DEF_NUM_LIST_ITEMS

// The directory popup stuff.
#define DEF_DIR_X					DEF_FILE_LIST_X
#define DEF_DIR_Y					DEF_FILE_LIST_Y - 25
#define DEF_DIR_W					DEF_FILE_LIST_W
#define DEF_DIR_H					DEF_LIST_ITEM_H

// Cancel & Ok Button locations.
#define DEF_CNCL_X	10
#define DEF_CNCL_Y	DEF_SELECT_Y - 40
#define DEF_OK_X		DEF_SELECT_W - 32 - DEF_CNCL_X
#define DEF_OK_Y		DEF_SELECT_H - 40

// And the label for the top of the d-box
#define DEF_LABEL_X	5
#define DEF_LABEL_Y	10
#define DEF_LABEL_W	DEF_SELECT_W - DEF_LABEL_X * 2
#define DEF_LABEL_H	18
	
// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
  
   
fileListItem::fileListItem(fileListBox* inList,pathItemType inType,char* inName)
	: drawGroup(1,1,DEF_LIST_ITEM_W,DEF_LIST_ITEM_H,fullClick) {

	int	numChars;
	
	ourList		= inList;
	ourType		= inType;
	ourName		= NULL;
	numChars = strlen(inName)+1;
	if (resizeBuff(numChars,&ourName)) {
		strcpy(ourName,inName);
	}
}


fileListItem::~fileListItem(void) {  resizeBuff(0,&ourName); }
	

// Custom draw() method for list items.	
void fileListItem::draw(void)	{

	if (ourList->isVisible(this)) {	// If we are visable..
		drawGroup::draw();				// We draw ourselves.
	}
	needRefresh = false;					// But in all cases. We no longer need to be drawn.
}

		
void fileListItem::drawSelf(void) { 
	
	colorObj aColor(LC_LIGHT_BLUE);
	bmpObj*	ourIcon;
	
	ourIcon = NULL;
	if (haveFocus()) {
		screen->fillRect(this,&aColor);
	} else {
		screen->fillRect(this,&white);
	}
	switch(ourType) {
		case noType			: ourIcon = NULL;						break;	
		case rootType		: ourIcon = NULL;						break;
		case folderType	: ourIcon = ourList->folderBmp;	break;
		case fileType		: ourIcon = ourList->docBmp;		break;
	}
	if (ourIcon) {
		ourIcon->setLocation(DEF_LIST_ICON_X+x,DEF_LIST_ICON_Y+y);
		ourIcon->drawSelf();
	}
	if (ourName) {
		ourList->itemLabel->setLocation(DEF_LIST_ITEM_TXT_X+x,DEF_LIST_ITEM_TXT_Y+y);
		ourList->itemLabel->setColors(&black);
		ourList->itemLabel->setValue(ourName);
		ourList->itemLabel->drawSelf();
	}
}


// Our item has been clicked! What the Hell do we do?
void fileListItem::doAction(void) {
		
	if (!haveFocus()) {													// If we DON't have focus (First click)..
		setFocusPtr(this);												// And grab the focus for ourselves.
		ourList->clickTimer->start();									// Start the double click timer.
	} else {																	// Else, we've been clicked before..
		if (!ourList->clickTimer->ding()) {							// If we're looking at a double click..			
			switch(ourType) {												// Lets see what we are..
				case rootType		:										// Really it can't be root..
				case folderType	:										// Double clicked a folder..
					ourList->ourFileDir->chooseFolder(ourName);	// Pass it to our list controller thing.
				break;														// we are done here. So long!
				case fileType	:											// Double clicked a file..
					ourList->ourFileDir->chooseFile(ourName);		// Pass it to our list controller thing.
				break;														// And again, we are done here.
				case noType		: break;									// Code is just broken. Give up.
			}
		} else {																// Else, this was a "later in time" click.
			ourList->clickTimer->start();								// Maybe a start of a new double click?
		}
	}
}


// We have finally gained focus, our life's ambition! And how do we deal with this?
void fileListItem::setThisFocus(bool setLoose) {

	drawGroup::setThisFocus(setLoose);
	if (setLoose) {
		ourList->ourFileDir->setItem(ourType,ourName);
	} else {
		ourList->ourFileDir->setItem(noType,NULL);
	}
}



// **************************************************************
// ******************* fileListBox stuff ************************
// **************************************************************


fileListBox::fileListBox(int x, int y, int width,int height,bool(*funct)(const char*))
	:scrollingList(x,y,width,height,touchScroll,dragEvents) {
	
	filterFx			= funct;
	ourFileDir		= NULL;
	folderBmp		= newStdLbl(DEF_LIST_ICON_X,DEF_LIST_ICON_Y,icon16,folderLbl);
	docBmp			= newStdLbl(DEF_LIST_ICON_X,DEF_LIST_ICON_Y,icon16,docLbl);
	itemLabel		= new label(DEF_LIST_ITEM_TXT_X,DEF_LIST_ITEM_TXT_Y,DEF_LIST_ITEM_TXT_W,DEF_LIST_ITEM_TXT_H,"no name",1);
	clickTimer		= new timeObj(DBL_CLICK_MS);
}


// Ohe only things that we need to delete are the three icons. Everything else is delt
// with automatically by the group list.
fileListBox::~fileListBox(void) {

	if (folderBmp) delete(folderBmp);
	if (docBmp) delete(docBmp);
	if (itemLabel) delete(itemLabel);
}	

// AFTER we've been created, the bossman fileDir is created. Now we get a link to that.
void	fileListBox::setFileDir(fileDir* inFileDir) {  ourFileDir = inFileDir; }


// As each path item is created its handed in here to check if its OK to add to the list.
bool fileListBox::checkFile(pathItem* trace) {
	
	if (filterFx) {
		return filterFx(trace);
	}
	return true;
}


// Those who control our lives from  behind the scenes have deemed.. It is time to refresh
// our list of path items. Lets get this done.
void fileListBox::fillList(void) {

	pathItem*		trace;
	fileListItem*	newListItem;
	
	dumpDrawObjList();																						// Dump anything we may have from before.
	if (ourFileDir) {																							// If we have a fileDir. (Sanity)
		trace = ourFileDir->getChildList();																// Grab a pointer to the first child.
		while(trace) {																							// While we have a non-NULL pointer..
			if (checkFile(trace)) {																			// Pass this child through the crucible of the user's filter function.
				newListItem = new fileListItem(this,trace->getType(),trace->getName());		// If this 
				if (newListItem) {																			// If we got a list item. (More sanity)
					addObj(newListItem);																		// Add this item to our list.
				} else {																							// Else, we didn't get one? really?
					trace = NULL;																				// Yikes! No RAM, fail.
				}
			}
			if (trace) trace = (pathItem*)trace->dllNext;											// Jump to the next item on the list.
		}
	}
	setNeedRefresh();																								
}


// Ah, finally they want to see us! Well at least the stage that this list is played on.
void fileListBox::drawSelf(void) { 

	rect	ourFrame;

	ourFrame.setRect(this);						// Set up a rect from our bounding rect.
	ourFrame.insetRect(-1);						// Basically puff up by one pixel.
	screen->drawRect(&ourFrame,&black);		// And draw a one pixel outline.
	screen->fillRect(this,&white);			// Fill ourselves with white.
}

 
   
// **************************************************************
// *********************** fileDir  stuff ***********************
// **************************************************************


// fileDir is the label on the dialog box showing what our current directory is. It is the
// filePath object so its basically the boss of the dialog box.
fileDir::fileDir(int inX, int inY, int inWidth,int inHeight,fileViewer* inViewer,fileListBox* inListBox)
	: drawGroup(inX,inY,inWidth,inHeight,fullClick),
	filePath() {
	
	ourViewer 		= inViewer;
	ourFileListBox	= inListBox;
	if (ourFileListBox) {
		ourFileListBox->setFileDir(this);
	}
	folderIcon = newStdLbl(DEF_LIST_ICON_X,-200,icon16,folderRetLbl);
	if (folderIcon) {
		addObj(folderIcon);
	}
	SDIcon = newStdLbl(DEF_LIST_ICON_X,-200,icon16,SDCardLbl);
	if (SDIcon) {
		addObj(SDIcon);
	}
	dirName = new label(DEF_LIST_ITEM_TXT_X,DEF_LIST_ITEM_TXT_Y,DEF_LIST_ITEM_TXT_W,DEF_LIST_ITEM_TXT_H);
	if (dirName) {
		dirName->setTextSize(1);
		addObj(dirName);
	}
	refresh();
}


// Our destructor. The only things we allocated were given to our viewList. Our viewlist
// will take care of deleting them. So, there's nothing really for us to do here.
fileDir::~fileDir(void) { 

	if (ourFileListBox) {
		ourFileListBox->setFileDir(NULL);
	}
}


// This is what we call to set the name & icon of our curent directory on the dialog box. 
void fileDir::refresh(void) {

	if (dirName && ourFileListBox) {												// If we have all our "parts"..
		if (!strcmp(getCurrItemName(),"/")) {									// If we're looking at root..
			dirName->setValue("SD Root");											// Set a more.. descriptive name for '/'.
			SDIcon->setLocation(DEF_LIST_ICON_X,DEF_LIST_ICON_Y);			// Set the SD icon in place.
			folderIcon->setLocation(DEF_LIST_ICON_X,-200);					// Move the folder icon offscreen.
		} else {																			// Else, we're looking at a folder..
			dirName->setValue(getCurrItemName());								// Set the new name for the current directory.
			SDIcon->setLocation(DEF_LIST_ICON_X,-200);						// Set the SD icon offscreen.
			folderIcon->setLocation(DEF_LIST_ICON_X,DEF_LIST_ICON_Y);	// Set the folder-return icon in place.
		}																					//
		ourFileListBox->fillList();												// Ok, we're all set, fill the file list box.
		setNeedRefresh();
	}										
}


// When its time to draw ourselves on the screen..
void fileDir::drawSelf(void) {

	colorObj aColor(LC_LIGHT_BLUE);

	if (haveFocus()) {						// If we have focus.. (been touched)
		screen->fillRect(this,&aColor);	// We paint our rectangle highlight blue.
	} else {										// Else, we don't have focus..
		screen->fillRect(this,&white);	// We paint our rect white.
	}												//
	screen->drawRect(this,&black);		// Lastly we frame our rect with a black outline.
}


// This is what is called when we receive a click from the user..
void fileDir::doAction(void) {
														// Else, we DID have focus..
	if (!strcmp(getCurrItemName(),"/")) {	// If we're looking at root..
		return;										// We just give up now.
	} else {											// Else, We double clicked a folder..
		popItem();									// Pop path to the parent directory.
		refresh();									// And do a refresh.
	}
}


// Descendants may need to do something when a path item is selected in the list box.
void fileDir::setItem(pathItemType inType,const char* name) { }


// Choosing folders should jump us to the next tier of folders.
void fileDir::chooseFolder(const char* name) {
	
	if (pushChildItemByName(name)) {
		refresh();
	}
}	

// We don't do the choose file thing. Descendants probably will..
void fileDir::chooseFile(const char* name) { }


// Let the calling function have a look at what should be the result path that we are
// showing on the screen.
char* fileDir::endChoice(void) {
	return getPath();							// For now, you get the path as it is.
}
		


// **************************************************************
// ********************* fileViewer stuff ***********************
// **************************************************************


fileViewer::fileViewer(listener* inListener,bool(*funct)(const char*))
	:alertObj("Default name",NULL,noIconAlert,true,true) {
	
	ourListener		= inListener;
	
	this->setRect(DEF_SELECT_X,DEF_SELECT_Y,DEF_SELECT_W,DEF_SELECT_H);
	
	theMsg->setRect(DEF_LABEL_X,DEF_LABEL_Y,DEF_LABEL_W,DEF_LABEL_H);
	
	okBtn->setLocation(DEF_OK_X,DEF_OK_Y);
	cancelBtn->setLocation(DEF_CNCL_X,DEF_CNCL_Y);
	
	ourListBox = new fileListBox(DEF_FILE_LIST_X,DEF_FILE_LIST_Y,DEF_FILE_LIST_W,FILE_LIST_HEIGHT,funct);
	addObj(ourListBox);	
	
	// Descendants will typically make custom versions of the fileDir objects.
	ourFileDir = new fileDir(DEF_DIR_X,DEF_DIR_Y,DEF_DIR_W,DEF_DIR_H,this,ourListBox);
	if (ourFileDir) {
		addObj(ourFileDir);
		ourFileDir->setPath("/");
		ourFileDir->refresh();
	}
}
	

// Everything we've created are draw Objectes. They will be deleted automatically when we
// are deleted.	
fileViewer::~fileViewer(void) {  }


// This is the default path we want to show.
void fileViewer::setPath(char* inPath) {

	ourFileDir->setPath(inPath); 
	ourFileDir->refresh();
}
	
// And we handle commands from our things.
void fileViewer::handleCom(stdComs comID) {  alertObj::handleCom(comID); }


// Whom ever created us can use this to read the result of our actions.
char*  fileViewer::getPathResult(void) {

	if (ourFileDir) {
		return ourFileDir->endChoice();
	}
	return NULL;
}


