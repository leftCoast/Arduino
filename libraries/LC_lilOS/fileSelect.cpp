#include <fileSelect.h>
#include <resizeBuff.h>
#include <adafruit_1947_Obj.h>

#define FILE_LIST_X			10
#define FILE_LIST_Y			50
#define FILE_LIST_WIDTH		100
#define NUM_LIST_ITEMS		5

#define ICON_X	2
#define ICON_Y	1
#define ICON_W	16
#define ICON_H	16

#define L_ITEM_W				FILE_LIST_WIDTH - 2
#define L_ITEM_H				18
#define L_ITEM_TXT_X			22
#define L_ITEM_TXT_Y			6
#define L_ITEM_TXT_W			FILE_LIST_WIDTH - L_ITEM_TXT_X - 2
#define L_ITEM_TXT_H			8


#define FILE_LIST_HEIGHT	L_ITEM_H * NUM_LIST_ITEMS

#define OPEN_X	30
#define OPEN_Y	40
#define OPEN_W	120
#define OPEN_H	190

#define FL_DIR_X	FILE_LIST_X
#define FL_DIR_Y	FILE_LIST_Y - 25
#define FL_DIR_W	FILE_LIST_WIDTH
#define FL_DIR_H	L_ITEM_H

#define CNCL_X	10
#define CNCL_Y	OPEN_H - 40
#define OK_X	OPEN_W - 32 - CNCL_X
#define OK_Y	OPEN_H - 40

#define LABEL_X	5
#define LABEL_Y	10
#define LABEL_W	OPEN_W - LABEL_X * 2
#define LABEL_H	18



timeObj	dblClickTimer(500);
bmpObj	folderBmp(ICON_X,ICON_Y,ICON_W,ICON_H,"/fldr16.bmp");
bmpObj	docBmp(ICON_X,ICON_Y,ICON_W,ICON_H,"/doc16.bmp");
label		itemLabel(L_ITEM_TXT_X,L_ITEM_TXT_Y,L_ITEM_TXT_W,L_ITEM_TXT_H,"no name",1);



// **************************************************************
// ************************ OKBtn stuff *************************
// **************************************************************

OKBtn::OKBtn(int xLoc,int yLoc,char* path,modal* inModal)
	: iconButton(xLoc,yLoc,path) { ourModal = inModal; }
	
OKBtn::~OKBtn(void) {  }
	
void OKBtn::doAction(void) { ourModal->setSuccess(true); }



// **************************************************************
// ********************** cancelBtn stuff ***********************
// **************************************************************


cancelBtn::cancelBtn(int xLoc,int yLoc,char* path,modal* inModal)
	: iconButton(xLoc,yLoc,path) { ourModal = inModal; }				

cancelBtn::~cancelBtn(void) {  }
	
void	cancelBtn::doAction(void) { ourModal->setSuccess(false); }


	
// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
   
fileListItem::fileListItem(fileBaseViewer* inViewer,fileListBox* inList,pathItemType inType,char* inName)
	: drawGroup(1,1,L_ITEM_W,L_ITEM_H,fullClick) {
	
	int	numChars;
	
	ourViewer	= inViewer;
	ourList		= inList;
	ourType		= inType;
	ourName		= NULL;
	numChars = strlen(inName)+1;
	if (resizeBuff(numChars,&ourName)) {
		strcpy(ourName,inName);
	}
}


fileListItem::~fileListItem(void) { resizeBuff(0,&ourName); }
	

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
	label*	ourLabel;
	
	if (haveFocus()) {
		screen->fillRect(this,&aColor);
	} else {
		screen->fillRect(this,&white);
	}
	switch(ourType) {
		case noType			: ourIcon = NULL;			break;
		case rootType		: ourIcon = NULL;			break;
		case folderType	: ourIcon = &folderBmp;	break;
		case fileType		: ourIcon = &docBmp;		break;
	}
	if (ourIcon) {
		ourIcon->setLocation(ICON_X+x,ICON_Y+y);
		ourIcon->drawSelf();
	}
	if (ourName) {
		itemLabel.setLocation(L_ITEM_TXT_X+x,L_ITEM_TXT_Y+y);
		itemLabel.setColors(&black);
		itemLabel.setValue(ourName);
		itemLabel.drawSelf();
	}
}


void fileListItem::doAction(void) {
		
	if (!haveFocus()) {
		dblClickTimer.start();
		setFocusPtr(this);
	} else {
		if (!dblClickTimer.ding()) {					// If we're looking at a double click..			
			switch(ourType) {								// Lets see what we are..
				case rootType		:						// Really it can't be..
				case folderType	:						// Double clicked a folder..
					ourViewer->chooseFolder(ourName);	// Pass it to our list controller thing.
				break;										// So long!
				case fileType	:							// Double clicked a file..
					ourViewer->chooseFile(ourName);		// Pass it to our list controller thing.
				break;										// So long!
				case noType		: break;					// Code is just broken. Give up.
			}
		} else {												// Else, this was not  double click..
			dblClickTimer.start();						// Restart the timer, maybe this is the start of a double?
		}
	}
}


void fileListItem::setThisFocus(bool setLoose) {

	drawGroup::setThisFocus(setLoose);
	if (setLoose) {
		ourViewer->setItem(this);
	} else {
		ourViewer->setItem(NULL);
	}
}


// **************************************************************
// ******************* fileListBox stuff ************************
// **************************************************************


fileListBox::fileListBox(int x, int y, int width,int height)
	:scrollingList(x,y,width,height,touchScroll,dragEvents) {  }


fileListBox::~fileListBox(void) { }	


bool fileListBox::checkFile(fileBaseViewer* ourPath,pathItem* trace) {
	
	if (ourPath->filterFx) {
		return ourPath->filterFx(trace->getName());
	}
	return true;
}


void fileListBox::fillList(fileBaseViewer* ourPath) {

	pathItem*		trace;
	fileListItem*	newListItem;
	
	dumpDrawObjList();
	if (ourPath) {
		trace = ourPath->childList;
		while(trace) {
			if (checkFile(ourPath,trace)) {
				newListItem = new fileListItem(ourPath,this,trace->getType(),trace->getName());
				if (newListItem) {
					addObj(newListItem);
				} else {
					trace = NULL;							// No r=RAM fail.
				}
			}
			if (trace) trace = (pathItem*)trace->dllNext;
		}
	}
	setNeedRefresh();
}


void fileListBox::drawSelf(void) { 

	rect	ourFrame;

	ourFrame.setRect(this);
	ourFrame.insetRect(-1);
	screen->drawRect(&ourFrame,&black);
	screen->fillRect(this,&white);
}

 
   
// **************************************************************
// *********************** fileDir  stuff ***********************
// **************************************************************


// fileDir is the label on the dialog box showing what our current directory is.
fileDir::fileDir(filePath* inPath,fileListBox* inFileListBox)
	: drawGroup(FL_DIR_X,FL_DIR_Y,FL_DIR_W,FL_DIR_H,fullClick) {
	
	ourPath			= inPath;
	ourFileListBox	= inFileListBox;
	folderIcon = new bmpObj(ICON_X,-200,ICON_W,ICON_H,"/fldrBk16.bmp");
	if (folderIcon) {
		addObj(folderIcon);
	}
	SDIcon = new bmpObj(ICON_X,-200,ICON_W,ICON_H,"/SD16.bmp");
	if (SDIcon) {
		addObj(SDIcon);
	}
	if (ourPath) {
		dirName = new label(L_ITEM_TXT_X,L_ITEM_TXT_Y,L_ITEM_TXT_W,L_ITEM_TXT_H);
		if (dirName) {
			dirName->setTextSize(1);
			addObj(dirName);
		}
		refresh();
	}
}


fileDir::~fileDir(void) {  }


// This is what we call to set the name & icon of our curent directory on the dialog box. 
void fileDir::refresh(void) {

	
	if (dirName && ourFileListBox) {																	// If we have a text obj for the dir name..
		if (!strcmp(ourPath->getCurrItemName(),"/")) {					// If we're looking at root..
			dirName->setValue("SD Root");										// Set a more.. descriptive name for '/'.
			SDIcon->setLocation(ICON_X,ICON_Y);
			folderIcon->setLocation(ICON_X,-200);
		} else {	
			dirName->setValue(ourPath->getCurrItemName());				// Set the new name for the current directory.
			SDIcon->setLocation(ICON_X,-200);
			folderIcon->setLocation(ICON_X,ICON_Y);
		}
		ourFileListBox->fillList(ourPath);
	}										
}


void fileDir::drawSelf(void) {

	colorObj aColor(LC_LIGHT_BLUE);
	
	if (haveFocus()) {
		screen->fillRect(this,&aColor);
	} else {
		screen->fillRect(this,&white);
	}
	screen->drawRect(this,&black);
}


void fileDir::doAction(void) {
	
	if (!haveFocus()) {												// If we DON'T have focus..
		dblClickTimer.start();										// Start the double click timer.
		setFocusPtr(this);											// And set focus to us.
	} else {																// Else, we DID have focus..
		if (!dblClickTimer.ding()) {								// If we're looking at a double click..			
			if (!strcmp(ourPath->getCurrItemName(),"/")) {	// If we're looking at root..
				return;													// We just give up now.
			} else {														// Else, We double clicked a folder..
				ourPath->popItem();									// Pop path to the parent directory.
				refresh();												// And do a refresh.
			}
		} else {															// Else, this was not  double click..
			dblClickTimer.start();									// Restart the timer, maybe this is the start of a double?
		}
	}
}


void fileDir::setThisFocus(bool setLoose) {

	colorObj aColor(LC_LIGHT_BLUE);
	
	if (setLoose) {
		dirName->setColors(&black,&aColor);
	} else {
		dirName->setColors(&black,&white);
	}
	drawObj::setThisFocus(setLoose);
}


// **************************************************************
// ******************* fileBaseViewer stuff *********************
// **************************************************************


fileBaseViewer::fileBaseViewer(panel* inPanel,bool(*funct)(char*))
	:modal(OPEN_X,OPEN_Y,OPEN_W,OPEN_H) {
	
	ourPanel = inPanel;
	filterFx	= funct;
	ourLabel = new label(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);
	if (ourLabel) {
		ourLabel->setTextSize(1);
		ourLabel->setJustify(TEXT_CENTER);
		ourLabel->setValue("Default name");
		addObj(ourLabel);
	}
	OKBtn* sBtn = new OKBtn(OK_X,OK_Y,ourPanel->mOSPtr->stdIconPath(check32),this);
	sBtn->setMask(&(ourPanel->mOSPtr->icon32Mask));
	addObj(sBtn);	
	cancelBtn* cBtn = new cancelBtn(CNCL_X,CNCL_Y,ourPanel->mOSPtr->stdIconPath(x32),this);
	cBtn->setMask(&(ourPanel->mOSPtr->icon32Mask));
	addObj(cBtn);	
	ourFileListBox = new fileListBox(FILE_LIST_X,FILE_LIST_Y,FILE_LIST_WIDTH,FILE_LIST_HEIGHT);
	setPath("/");
	addObj(ourFileListBox);	
	ourFileDir = new fileDir(this,ourFileListBox);
	if (ourFileDir) {
		addObj(ourFileDir);
		ourFileDir->refresh();
	}
}
	

// Everything we've created are draw Objectes. They will be deleted automatically when we
// are deleted.	
fileBaseViewer::~fileBaseViewer(void) {  }


// Choosing folders should jump us to the next tier of folders.
void fileBaseViewer::chooseFolder(char* name) {

	if (pushChildItemByName(name)) {
		ourFileDir->refresh();
	}
}	

// we do the choose file thing. Default to doing nothing?
void fileBaseViewer::chooseFile(char* name) {  }

// Needed as a pass through.		
void fileBaseViewer::setSuccess(bool trueFalse)	{ modal::setSuccess(trueFalse); }


// If a list item gets highlighted, we save a pointer to it in case its the last thing we do.	
void fileBaseViewer::setItem(fileListItem* currentSelected) { currentItem = currentSelected; }

	
// Basically we are nothing but big white rectangle.
void fileBaseViewer::drawSelf(void) {

	x++;
	y++;
	screen->drawRect(this,&black);
	x--;
	y--;
	screen->fillRect(this,&white);
	screen->drawRect(this,&black);
}

	
// **************************************************************
// *********************** fOpenObj stuff ***********************
// **************************************************************



fOpenObj::fOpenObj(panel* inPanel,bool(*funct)(char*)=NULL)
	:fileBaseViewer(inPanel,funct) {  }


fOpenObj::~fOpenObj(void) { }


// Someone double clicked on a file. Open this one!
void fOpenObj::chooseFile(char* name) {
	
	currentItem = NULL;				// On a double click we already have the name. Clear this.
	pushChildItemByName(name);		// Push the clicked name into the path.
	setSuccess(true);					// And we call success to end this.
}


// Pass this to modal. The different sub classes will use this.
void fOpenObj::setSuccess(bool trueFalse) {

	if (currentItem && trueFalse) {							// If we have a current item and succes will be true..
		if (currentItem->ourType==fileType) {				// If this item is a file..
			pushChildItemByName(currentItem->ourName);	// Push it onto the path.
		}
	}
	fileBaseViewer::setSuccess(trueFalse);					// Call the inherited to unwind everthing.
}				



// **************************************************************
// ********************** fSaveObj stuff ************************
// **************************************************************


fSaveObj::fSaveObj(panel* inPanel,bool(*funct)(char*)=NULL)
	:fileBaseViewer(inPanel,funct) {  }
	
fSaveObj::~fSaveObj(void) {  }


// Pass this to modal. The different sub classes will use this.
void fSaveObj::setSuccess(bool trueFalse) {

	fileBaseViewer::setSuccess(trueFalse);

}


