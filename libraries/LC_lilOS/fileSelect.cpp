#include <fileSelect.h>
#include <resizeBuff.h>
#include <adafruit_1947_Obj.h>

#define FILE_LIST_X			10
#define FILE_LIST_Y			30
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
#define OPEN_W	180
#define OPEN_H	170

#define FL_DIR_X	FILE_LIST_X
#define FL_DIR_Y	FILE_LIST_Y - 25
#define FL_DIR_W	FILE_LIST_WIDTH
#define FL_DIR_H	L_ITEM_H


// Eww, this is a scary one! This allocates a #define ICON_X list icon for display. It hands it to
// YOU with the implied agreement that : YOU will make sure that, when it's job is done,
// it is recycled.
bmpObj* createListIcon(pathItemType inType) {
	
	bmpObj*	ourIcon;
	
	if (inType==folderType) {
		ourIcon = new bmpObj(ICON_X,ICON_Y,ICON_W,ICON_H,"/fldr16.bmp");
	} else if (inType==fileType) {
		ourIcon = new bmpObj(ICON_X,ICON_Y,ICON_W,ICON_H,"/doc16.bmp");
	} else /*if ((inType==rootType) */{
		ourIcon = new bmpObj(ICON_X,ICON_Y,ICON_W,ICON_H,"/SD16.bmp");
	}
	return ourIcon;
}


#define CNCL_X	5
#define CNCL_Y	130
#define OK_X	42
#define OK_Y	130

// **************************************************************
// ************************ OKBtn stuff *************************
// **************************************************************

OKBtn::OKBtn(int xLoc,int yLoc,char* path,modal* inModal)
	: iconButton(xLoc,yLoc,path) { ourModal = inModal; }
	
OKBtn::~OKBtn(void) {  }
	
void OKBtn::doAction(void) {

	ourModal->success = true;
	ourModal->done = true;
}



// **************************************************************
// ********************** cancelBtn stuff ***********************
// **************************************************************


cancelBtn::cancelBtn(int xLoc,int yLoc,char* path,modal* inModal)
	: iconButton(xLoc,yLoc,path) { ourModal = inModal; }				

cancelBtn::~cancelBtn(void) {  }
	
void	cancelBtn::doAction(void) {

	ourModal->success = false;
	ourModal->done = true;
}


	
// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
   
fileListItem::fileListItem(fileBaseViewer* inViewer,fileListBox* inList,pathItemType inType,char* inName)
	: drawGroup(1,1,L_ITEM_W,L_ITEM_H,fullClick) {
	
	bmpObj*	ourIcon;
	
	ourViewer	= inViewer;
	ourList		= inList;
	ourType		= inType;
	fileName		= new label(L_ITEM_TXT_X,L_ITEM_TXT_Y,L_ITEM_TXT_W,L_ITEM_TXT_H,inName,1);
	if (fileName) {
		fileName->setColors(&black);
		addObj(fileName);
	}
	ourIcon = createListIcon(inType);
	addObj(ourIcon);
}


fileListItem::~fileListItem(void) { }
	
	
void fileListItem::draw(void)	{

	if (ourList->isVisible(this)) {
		Serial.println("Drawing");
		drawGroup::draw();
	}
	needRefresh = false;
}

		
void fileListItem::drawSelf(void) { 
	
	colorObj aColor(LC_LIGHT_BLUE);
	
	if (haveFocus()) {
		screen->fillRect(this,&aColor);
	} else {
		screen->fillRect(this,&white);
	}
}



void fileListItem::doAction(void) {
	
	char name[13];
	
	if (!haveFocus()) {
		dblClickTimer.start();
		setFocusPtr(this);
	} else {
		if (!dblClickTimer.ding()) {					// If we're looking at a double click..
			fileName->getText(name);					// Grab the saved off name text.
			switch(ourType) {								// Lets see what we are..
				case rootType		:						// Really it can't be..
				case folderType	:						// Double clicked a folder..
					ourViewer->chooseFolder(name);	// Pass it to our list controller thing.
				break;										// So long!
				case fileType	:							// Double clicked a file..
					ourViewer->chooseFile(name);		// Pass it to our list controller thing.
				break;										// So long!
				case noType		: break;					// Code is just broken. Give up.
			}
		} else {												// Else, this was not  double click..
			dblClickTimer.start();						// Restart the timer, maybe this is the start of a double?
		}
	}
}

	
// **************************************************************
// ******************* fileListBox stuff ************************
// **************************************************************


fileListBox::fileListBox(int x, int y, int width,int height)
	:scrollingList(x,y,width,height,touchScroll,dragEvents) {  }


fileListBox::~fileListBox(void) { }	


void fileListBox::fillList(fileBaseViewer* ourPath) {

	pathItem*		trace;
	fileListItem*	newListItem;
	int				numItems;
	
	dumpDrawObjList();
	if (ourPath) {
		numItems = ourPath->numChildItems();
		for (int i=0;i<numItems;i++) {
			trace = ourPath->getChildItemByIndex(i);
			if (trace) {
				newListItem = new fileListItem(ourPath,this,trace->getType(),trace->getName());
				addObj(newListItem);
			}
		}
	}
}


void fileListBox::drawSelf(void) { 

	rect	ourFrame;

	ourFrame.setRect(this);
	ourFrame.insetRect(-1);
	screen->drawRect(&ourFrame,&black);
}

 
   
// **************************************************************
// *********************** fileDir  stuff ***********************
// **************************************************************

// fileDir is the label on the dialog box showing what our current directory is.


fileDir::fileDir(filePath* inPath)
	: drawGroup(FL_DIR_X,FL_DIR_Y,FL_DIR_W,FL_DIR_H) {
	
	ourPath = inPath;
	ourIcon = NULL;
	if (ourPath) {
		dirName = new label(L_ITEM_TXT_X,L_ITEM_TXT_Y,L_ITEM_TXT_W,L_ITEM_TXT_H);
		if (dirName) {
			dirName->setTextSize(1);
			addObj(dirName);
		}
	}
}


fileDir::~fileDir(void) {  }


// This is what we call to set the name & icon of our curent directory on the dialog box. 
void fileDir::refresh(void) {

	
	if (ourPath) {																		// If we have non-NULL file path..
		if (dirName) {																	// If we have a text obj for the dir name..
			if (!strcmp(ourPath->getCurrItemName(),"/")) {					// If we're looking at root..
				dirName->setValue("SD Root");										// Set a more.. descriptive name for '/'.
			} else {	
				dirName->setValue(ourPath->getCurrItemName());				// Set the new name for the current directory.
			}
		} 
		if (ourIcon) {																	// If we have an icon obj..
			delete (ourIcon);															// Delete it.
		}
		ourIcon = createListIcon(ourPath->getCurrItem()->getType());	// Create a new one with whatever type we now have.
		addObj(ourIcon);																// Add the new icon.
	}
}


void	 fileDir::drawSelf(void) { screen->drawRect(this,&black); }



// **************************************************************
// ******************* fileBaseViewer stuff *********************
// **************************************************************


fileBaseViewer::fOpenObj(panel* inPanel)
	:modal(OPEN_X,OPEN_Y,OPEN_W,OPEN_H) {
	
	ourPanel = inPanel;
	filterFx	= NULL;
	
	OKBtn* sBtn = new OKBtn(OK_X,OK_Y,ourPanel->mOSPtr->stdIconPath(check32),this);
	sBtn->setMask(&(ourPanel->mOSPtr->icon32Mask));
	addObj(sBtn);
	
	cancelBtn* cBtn = new cancelBtn(CNCL_X,CNCL_Y,ourPanel->mOSPtr->stdIconPath(x32),this);
	cBtn->setMask(&(ourPanel->mOSPtr->icon32Mask));
	addObj(cBtn);
	
	ourFileListBox = new fileListBox(FILE_LIST_X,FILE_LIST_Y,FILE_LIST_WIDTH,FILE_LIST_HEIGHT);
	setPath("/");
	ourFileListBox->fillList(this);
	addObj(ourFileListBox);
	
	ourFileDir = new fileDir(this);
	if (ourFileDir) {
		ourFileDir->refresh();
		addObj(ourFileDir);
	}
	

// Everything we've created are draw Objectes. They will be deleted automatically when we
// are deleted.	
fileBaseViewer::~fileBaseViewer(void) {  }


// Choosing folders should jump us to the next tier of folders.
void fOpenObj::chooseFolder(char* name) {

	pushChildItemByName(name);
	ourFileListBox->fillList(this);
}


// we don't do anything when choosing a file. Inherited probably will.
void fOpenObj::chooseFile(char* name) { }				
	
							
// Use a callback to filter what you see.
void fOpenObj::setFilterCallback(bool(*funct)(char*)) { filterFx = funct; }


// Basically we are nothing but big white rectangle.
void fOpenObj::drawSelf(void) { screen->fillRect(this,&white); }

	
// **************************************************************
// *********************** fOpenObj stuff ***********************
// **************************************************************



fOpenObj::fOpenObj(panel* inPanel)
	:fileBaseViewer(inPanel) {  }


fOpenObj::~fOpenObj(void) {
	
	gMask = NULL;
	if (ourModalMask) delete(ourModalMask);
}


void fOpenObj::chooseFile(char* name) {

}				



// **************************************************************
// ********************** fSaveObj stuff ************************
// **************************************************************


fSaveObj::fSaveObj(panel* inPanel)
	:fileBaseViewer(inPanel) {  }
	
fSaveObj::~fSaveObj(void) {  }




