#include <fSaveObj.h>
#include <resizeBuff.h>
#include <bmpKeyboard.h>
#include <editLabel.h>

#define out(x) Serial.print(x)
#define outln(x) Serial.println(x)

// The overall size and placement of the save d-box.
#define SAVE_X			0
#define SAVE_Y			24
#define SAVE_W			240
#define SAVE_H			320 - SAVE_Y

// The box we actually see.
#define SAVE_SCR_X	30
#define SAVE_SCR_Y	30
#define SAVE_SCR_W	160
#define SAVE_SCR_H	155

// And the label for the top of the d-box
#define LABEL_X		SAVE_SCR_X + 5
#define LABEL_Y		12
#define LABEL_W		SAVE_SCR_W - 10
#define LABEL_H		18

// The editable file name.
#define NAME_STR_X	SAVE_SCR_X + 10
#define NAME_STR_Y	145
#define NAME_STR_W	SAVE_SCR_W - 20
#define NAME_STR_H	18

// File list, size & num items.
#define FILE_LIST_X	SAVE_SCR_X + 10
#define FILE_LIST_Y	50

// The directory popup stuff.
#define DIR_OBJ_X	FILE_LIST_X
#define DIR_OBJ_Y	FILE_LIST_Y - 25

// New folder button.
#define NEW_FLDR_X	150	
#define NEW_FLDR_Y	FILE_LIST_Y

#define DELETE_X		NEW_FLDR_X
#define DELETE_Y		NEW_FLDR_Y + 45

// **************************************************************
// ********************** newFolderBtn stuff ***********************
// **************************************************************


newFolderBtn::newFolderBtn(int xLoc,int yLoc,char* path,modal* inModal)
	: iconButton(xLoc,yLoc,path) { ourModal = inModal; }
	
	
newFolderBtn::~newFolderBtn(void) {  }

	
void newFolderBtn::doAction(void) {

}
	


// **************************************************************
// ********************** deleteBtn stuff ***********************
// **************************************************************


deleteBtn::deleteBtn(int xLoc,int yLoc,char* path,modal* inModal)
	: iconButton(xLoc,yLoc,path) { ourModal = inModal; }
	
deleteBtn::~deleteBtn(void) {  }

	
void deleteBtn::doAction(void) {

}



// **************************************************************
// ********************* saveKbd stuff *************************
// **************************************************************


saveKbd::saveKbd(editable* inEditObj,fSaveObj* inModal)
	: bmpKeyboard(inEditObj,true) { ourModal = inModal; }

	
saveKbd::~saveKbd(void) {  }

	
void saveKbd::handleKey(keyCommands inEditCom) {

	if (inEditCom==cancel) {
		ourModal->setSuccess(false);
	} else if (inEditCom==ok) {
		ourModal->setSuccess(true);
	}
	bmpKeyboard::handleKey(inEditCom);
}


// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************

	
fSaveObj::fSaveObj(panel* inPanel,bool(*funct)(char*))
	:fileBaseViewer(inPanel,funct) {
	
	int kbdX;
	int kbdY;
	
	savePath = NULL;
	this->setRect(SAVE_X,SAVE_Y,SAVE_W,SAVE_H);																			// Resize our window.
	ourLabel->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);																// Resize the label to fit.
	ourLabel->setValue("Choose save location");																			// Set the label name.
	ourLabel->setJustify(TEXT_CENTER);																						// Center to make it look good.
	if (sBtn) {																														// If we have a default success button..
		delete(sBtn);																												// Delete it.
		sBtn = NULL;																												// And NULL it out as a flag.
	}
	if (cBtn) {																														// If we have a default cancel button..
		delete(cBtn);																												// Delete it.
		cBtn = NULL;																												// And NULL it out as a flag.
	}
	pathStr = NULL;
	ourFileListBox->setLocation(FILE_LIST_X,FILE_LIST_Y);
	ourFileDir->setLocation(DIR_OBJ_X,DIR_OBJ_Y);																										// Hook it into the drawObject ist.
	nameStr = new editLabel(NAME_STR_X,NAME_STR_Y,NAME_STR_W,NAME_STR_H);										// Create a name string obj.
	addObj(nameStr);																												// Hook it into the drawObject ist.
	setName("starTrek.bmp");																									// Set the name to a default for now.
	folderBtn = new newFolderBtn(NEW_FLDR_X,NEW_FLDR_Y,ourPanel->mOSPtr->stdIconPath(FdrNew32),this);
	folderBtn->setMask(&(ourPanel->mOSPtr->icon32Mask));
	addObj(folderBtn);
	delBtn = new deleteBtn(DELETE_X,DELETE_Y,ourPanel->mOSPtr->stdIconPath(trashC32),this);
	delBtn->setMask(&(ourPanel->mOSPtr->icon32Mask));
	addObj(delBtn);
	saveKbd* aKbd = new saveKbd(nameStr,this);
	kbdX = aKbd->x;
	kbdY = aKbd->y;
	aKbd->setLocation(kbdX,kbdY-22);
	aKbd->loadKeys();
	addObj(aKbd);
	nameStr->beginEditing();
	showPath();	
}
	

fSaveObj::~fSaveObj(void) { resizeBuff(0,&savePath); }


void fSaveObj::setName(char* inName) {

	if (nameStr) {
		nameStr->setValue(inName);
	}
}


void fSaveObj::showPath(void) { 

	if (pathStr) {
		pathStr->setValue(getPath());
	}
}


void fSaveObj::reset(void) {

	fileBaseViewer::reset();
	showPath();
}

bool fSaveObj::setPath(char* inPath) {
	
	fileBaseViewer::setPath(inPath);
	showPath();
}


bool fSaveObj::pushItem(pathItem* theNewGuy) {
	
	fileBaseViewer::pushItem(theNewGuy);
	showPath();
}


void fSaveObj::popItem(void) {

	fileBaseViewer::popItem();
	showPath();
}
	
	 
// The file viewer is going down either with a success or a failure. Deal with that here.
void fSaveObj::setSuccess(bool trueFalse) {

	int	numBytes;
	char*	fileName;
	
	fileName = NULL;
	if (trueFalse) {
		if (resizeBuff(nameStr->getNumChars()+1,&fileName)) {
			nameStr->getText(fileName);
			numBytes = numPathBytes() + strlen(fileName);
			if (resizeBuff(numBytes,&savePath)) {
				strcpy(savePath,getPath());
				strcat(savePath,fileName);
				outln(savePath);
			}
		}
	}	
	fileBaseViewer::setSuccess(trueFalse);
}


char* fSaveObj::getSavePath(void) { return savePath; }


// Basically we are nothing but big white rectangle.
void fSaveObj::drawSelf(void) {
	
	rect	aRect(SAVE_SCR_X,SAVE_SCR_Y,SAVE_SCR_W,SAVE_SCR_H);
	
	aRect.x++;										// Quick move it over one click in x.
	aRect.y++;										// One click in y.
	screen->drawRect(&aRect,&black);	// One pixal rectangle makes our drop shadow.
	aRect.x--;										// Reset x.
	aRect.y--;										// Reset y.
	screen->fillRect(&aRect,&white);	// Fill our rectangle white.
	screen->drawRect(&aRect,&black);	// And draw  black outline for us.
	
	aRect.setRect(0,220,240,100);
	screen->fillRect(&aRect,&black);
}


