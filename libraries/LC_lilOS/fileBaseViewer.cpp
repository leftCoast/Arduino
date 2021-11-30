#include <fileBaseViewer.h>
#include <resizeBuff.h>
#include <adafruit_1947_Obj.h>

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
#define DEF_DIR_OBJ_X	DEF_FILE_LIST_X
#define DEF_DIR_OBJ_Y	DEF_FILE_LIST_Y - 25
#define DEF_DIR_OBJ_W	DEF_FILE_LIST_W
#define DEF_DIR_OBJ_H	DEF_LIST_ITEM_H

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
  
   
fileListItem::fileListItem(fileBaseViewer* inViewer,fileListBox* inList,pathItemType inType,char* inName)
	: drawGroup(1,1,DEF_LIST_ITEM_W,DEF_LIST_ITEM_H,fullClick) {
	
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


void fileListItem::doAction(void) {
		
	if (!haveFocus()) {										// If we DON't have focus (First click)..
		ourViewer->dblClickTimer->start();				// we start our double click timer.
		setFocusPtr(this);									// And grab the focus for ourselves.
	} else {														// Else, we've been clicked before..
		if (!ourViewer->dblClickTimer->ding()) {		// If we're looking at a double click..			
			switch(ourType) {									// Lets see what we are..
				case rootType		:							// Really it can't be root..
				case folderType	:							// Double clicked a folder..
					ourViewer->chooseFolder(ourName);	// Pass it to our list controller thing.
				break;											// we are done here. So long!
				case fileType	:								// Double clicked a file..
					ourViewer->chooseFile(ourName);		// Pass it to our list controller thing.
				break;											// And again, we are done here.
				case noType		: break;						// Code is just broken. Give up.
			}
		} else {													// Else, this was not  double click.. (Just another click on a hilighted item.)
			ourViewer->dblClickTimer->start();			// Restart the timer. Maybe this is the start of a double?
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
	:scrollingList(x,y,width,height,touchScroll,dragEvents) {
	
	folderBmp		= new bmpObj(DEF_LIST_ICON_X,DEF_LIST_ICON_Y,DEF_LIST_ICON_W,DEF_LIST_ICON_H,"/fldr16.bmp");
	docBmp			= new bmpObj(DEF_LIST_ICON_X,DEF_LIST_ICON_Y,DEF_LIST_ICON_W,DEF_LIST_ICON_H,"/doc16.bmp");
	itemLabel		= new label(DEF_LIST_ITEM_TXT_X,DEF_LIST_ITEM_TXT_Y,DEF_LIST_ITEM_TXT_W,DEF_LIST_ITEM_TXT_H,"no name",1);
}


fileListBox::~fileListBox(void) {

	if (folderBmp) delete(folderBmp);
	if (docBmp) delete(docBmp);
	if (itemLabel) delete(itemLabel);
}	


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
					trace = NULL;							// No RAM, fail.
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
fileDir::fileDir(fileBaseViewer* inViewer,fileListBox* inFileListBox)
	: drawGroup(DEF_DIR_OBJ_X,DEF_DIR_OBJ_Y,DEF_DIR_OBJ_W,DEF_DIR_OBJ_H,fullClick) {
	
	ourViewer		= inViewer;
	ourFileListBox	= inFileListBox;
	folderIcon = new bmpObj(DEF_LIST_ICON_X,-200,DEF_LIST_ICON_W,DEF_LIST_ICON_H,"/fldrBk16.bmp");
	if (folderIcon) {
		addObj(folderIcon);
	}
	SDIcon = new bmpObj(DEF_LIST_ICON_X,-200,DEF_LIST_ICON_W,DEF_LIST_ICON_H,"/SD16.bmp");
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


fileDir::~fileDir(void) {  }


// This is what we call to set the name & icon of our curent directory on the dialog box. 
void fileDir::refresh(void) {

	
	if (dirName && ourFileListBox) {								// If we have a text obj for the dir name..
		if (!strcmp(ourViewer->getCurrItemName(),"/")) {		// If we're looking at root..
			dirName->setValue("SD Root");							// Set a more.. descriptive name for '/'.
			SDIcon->setLocation(DEF_LIST_ICON_X,DEF_LIST_ICON_Y);
			folderIcon->setLocation(DEF_LIST_ICON_X,-200);
		} else {	
			dirName->setValue(ourViewer->getCurrItemName());	// Set the new name for the current directory.
			SDIcon->setLocation(DEF_LIST_ICON_X,-200);
			folderIcon->setLocation(DEF_LIST_ICON_X,DEF_LIST_ICON_Y);
		}
		ourFileListBox->fillList(ourViewer);
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
	
	if (!haveFocus()) {													// If we DON'T have focus..
		ourViewer->dblClickTimer->start();							// Start the double click timer.
		setFocusPtr(this);												// And set focus to us.
	} else {																	// Else, we DID have focus..
		if (!ourViewer->dblClickTimer->ding()) {					// If we're looking at a double click..			
			if (!strcmp(ourViewer->getCurrItemName(),"/")) {	// If we're looking at root..
				return;														// We just give up now.
			} else {															// Else, We double clicked a folder..
				ourViewer->popItem();									// Pop path to the parent directory.
				refresh();													// And do a refresh.
			}
		} else {																// Else, this was not  double click..
			ourViewer->dblClickTimer->start();						// Restart the timer, maybe this is the start of a double?
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
	:modal(DEF_SELECT_X,DEF_SELECT_Y,DEF_SELECT_W,DEF_SELECT_H) {
	
	ourPanel = inPanel;
	filterFx	= funct;
	ourLabel = new label(DEF_LABEL_X,DEF_LABEL_Y,DEF_LABEL_W,DEF_LABEL_H);
	if (ourLabel) {
		ourLabel->setTextSize(1);
		ourLabel->setJustify(TEXT_CENTER);
		ourLabel->setValue("Default name");
		addObj(ourLabel);
	}
	sBtn = newStdBtn(DEF_OK_X,DEF_OK_Y,icon32,okCmd,this);
	addObj(sBtn);
	cBtn = newStdBtn(DEF_CNCL_X,DEF_CNCL_Y,icon32,cancelCmd,this);
	addObj(cBtn);	
	ourFileListBox = new fileListBox(DEF_FILE_LIST_X,DEF_FILE_LIST_Y,DEF_FILE_LIST_W,FILE_LIST_HEIGHT);
	setPath("/");
	addObj(ourFileListBox);	
	ourFileDir = new fileDir(this,ourFileListBox);
	if (ourFileDir) {
		addObj(ourFileDir);
		ourFileDir->refresh();
	}
	dblClickTimer	= new timeObj(500);
}
	

// Everything we've created are draw Objectes. They will be deleted automatically when we
// are deleted.	
fileBaseViewer::~fileBaseViewer(void) { if (dblClickTimer) delete(dblClickTimer); }


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

	x++;										// Quick move it over one click in x.
	y++;										// One click in y.
	screen->drawRect(this,&black);	// One pixal rectangle makes our drop shadow.
	x--;										// Reset x.
	y--;										// Reset y.
	screen->fillRect(this,&white);	// Fill our rectangle white.
	screen->drawRect(this,&black);	// And draw  black outline for us.
}


void  fileBaseViewer::handleCom(stdComs comID) {

	switch(comID) {
		case cancelCmd	: setSuccess(false); break;
		case okCmd		: setSuccess(true); break;
		default			: break;
	}
}
