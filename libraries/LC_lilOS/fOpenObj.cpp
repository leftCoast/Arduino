#include <fOpenObj.h>
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
#define OPEN_H	140

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

	
// **************************************************************
// ********************* fileListItem stuff *********************
// **************************************************************
   
fileListItem::fileListItem(fileList* inList,pathItemType inType,char* inName)
	: drawGroup(1,1,L_ITEM_W,L_ITEM_H,fullClick) {
	
	bmpObj*	ourIcon;
	
	ourList	= inList;
	ourType	= inType;
	fileName = new label(L_ITEM_TXT_X,L_ITEM_TXT_Y,L_ITEM_TXT_W,L_ITEM_TXT_H,inName,1);
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
		drawGroup::draw();
	}
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
	
	if (!haveFocus()) {
		setFocusPtr(this);
	} else {
		//ourList->pushChildItemByName(fileName);
		setFocusPtr(NULL);
	}
}

	
// **************************************************************
// ********************* fileList stuff *************************
// **************************************************************


fileList::fileList(int x, int y, int width,int height)
	:scrollingList(x,y,width,height,touchScroll,dragEvents) {
	
}


fileList::~fileList(void) { }	


void fileList::fillList(filePath* ourPath) {

	pathItem*		trace;
	fileListItem*	newListItem;
	int				numItems;
	
	dumpDrawObjList();
	if (ourPath) {
		numItems = ourPath->numChildItems();
		for (int i=0;i<numItems;i++) {
			trace = ourPath->getChildItemByIndex(i);
			if (trace) {
				newListItem = new fileListItem(this,trace->getType(),trace->getName());
				addObj(newListItem);
			}
		}
	}
}

void fileList::drawSelf(void) {

	screen->drawRect(this,&black);
}
   
 
   
// **************************************************************
// ********************* fileListDir  stuff *********************
// **************************************************************


fileListDir::fileListDir(filePath* inPath)
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


fileListDir::~fileListDir(void) {  }


void fileListDir::refresh(void) {

	
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

/*				
void fileListDir::draw(void) {

	maskRect* ourMask;
	
	ourMask = (maskRect*)gMask;
	ourMask->setInverse(true);
	drawGroup::draw();
	ourMask->setInverse(false);
}
*/

void fileListDir::drawSelf(void) { }

// **************************************************************
// *********************** fOpenObj stuff ***********************
// **************************************************************


fOpenObj::fOpenObj(void)
	:drawGroup(OPEN_X,OPEN_Y,OPEN_W,OPEN_H) {
	
	filterFx	= NULL;
	resultFx	= NULL;
	
	setEventSet(fullClick);
	fileListBox = new fileList(FILE_LIST_X,FILE_LIST_Y,FILE_LIST_WIDTH,FILE_LIST_HEIGHT);
	setPath("/");
	fileListBox->fillList(this);
	addObj(fileListBox);
	ourFileListDir = new fileListDir(this);
	if (ourFileListDir) {
		ourFileListDir->refresh();
		addObj(ourFileListDir);
	}
	ourModalMask = new maskRect(this);
	gMask = ourModalMask;
}


fOpenObj::~fOpenObj(void) {
	
	if (ourModalMask) delete(ourModalMask);
	gMask = NULL;
}


			
// Use a callback to filter what you see.
void fOpenObj::setFilterCallback(bool(*funct)(char*)) { filterFx = funct; }


// Get a call back with the result.
void fOpenObj::setResultCallback(void(*funct)(bool))  { resultFx = funct; }


void fOpenObj::draw(void) {

	ourModalMask->setInverse(true);
	drawGroup::draw();
	ourModalMask->setInverse(false);
}

void fOpenObj::drawSelf(void) {

	screen->fillRect(this,&white);
}

void fOpenObj::doAction(void) {  }