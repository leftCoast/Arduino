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
// ********************* saveKbd stuff *************************
// **************************************************************


saveKbd::saveKbd(editable* inEditObj,fSaveObj* inModal)
	: bmpKeyboard(inEditObj,true) { ourModal = inModal; }

	
saveKbd::~saveKbd(void) {  }

	
void saveKbd::handleKey(keyCommands inEditCom) {

	if (inEditCom==cancel) {
		ourModal->handleCom(cancelCmd);
	} else if (inEditCom==ok) {
		ourModal->handleCom(okCmd);
	}
	bmpKeyboard::handleKey(inEditCom);
}


// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************

fSaveObj* staticThis;

	
fSaveObj::fSaveObj(panel* inPanel,bool(*funct)(char*))
	:fileBaseViewer(inPanel,funct) {
	
	int kbdX;
	int kbdY;
	stdComBtn*	folderBtn;
	stdComBtn*	delBtn;
	
	staticThis = this;
	savePath = NULL;
	ourAlert = NULL;
	wereDoing = choosing;
	this->setRect(SAVE_X,SAVE_Y,SAVE_W,SAVE_H);											// Resize our window.
	ourLabel->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);								// Resize the label to fit.
	ourLabel->setValue("Choose save location");											// Set the label name.
	ourLabel->setJustify(TEXT_CENTER);														// Center to make it look good.
	if (sBtn) {																						// If we have a default success button..
		delete(sBtn);																				// Delete it.
		sBtn = NULL;																				// And NULL it out as a flag.
	}
	if (cBtn) {																						// If we have a default cancel button..
		delete(cBtn);																				// Delete it.
		cBtn = NULL;																				// And NULL it out as a flag.
	}
	ourFileListBox->setLocation(FILE_LIST_X,FILE_LIST_Y);
	ourFileDir->setLocation(DIR_OBJ_X,DIR_OBJ_Y);										// Hook it into the drawObject ist.
	nameStr = new editLabel(NAME_STR_X,NAME_STR_Y,NAME_STR_W,NAME_STR_H);		// Create a name string obj.
	addObj(nameStr);																				// Hook it into the drawObject ist.
	setName("noName.bmp");																		// Set the name to a default for now.
	folderBtn = newStdBtn(NEW_FLDR_X,NEW_FLDR_Y,icon32,newFolderCmd,this);
	addObj(folderBtn);
	delBtn = newStdBtn(DELETE_X,DELETE_Y,icon32,deleteItemCmd,this);
	addObj(delBtn);
	aKbd = new saveKbd(nameStr,this);
	kbdX = aKbd->x;
	kbdY = aKbd->y;
	aKbd->setLocation(kbdX,kbdY-22);
	aKbd->loadKeys();
	addObj(aKbd);
	nameStr->beginEditing();
}
	

fSaveObj::~fSaveObj(void) { resizeBuff(0,&savePath); }


void fSaveObj::setName(char* inName) {

	if (nameStr) {
		if (nameStr->mEditing) {
			nameStr->endEditing();
			nameStr->setValue(inName);
			nameStr->beginEditing();
		} else {
			nameStr->setValue(inName);
		}
	}
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


void fSaveObj::setItem(fileListItem* currentSelected) {

	fileBaseViewer::setItem(currentSelected);
	if (currentSelected->ourType==fileType) {
		nameStr->endEditing();
		nameStr->setValue(currentSelected->ourName);
		nameStr->beginEditing();
	}
}


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


void	fSaveObj::newFolderAlert(void) {
	
	staticThis->ourAlert = new alertObj("Enter new folder name.",staticThis,choiceAlert,false,false);
	staticThis->addObj(staticThis->ourAlert);
	staticThis->setMode(newFolder);
}


// This deletes the selected file from the SD card.
void	fSaveObj::deleteFile() {

	if (pushChildItemByName(currentItem->ourName)) {	// Add the selected name to the end of the path.
		SD.remove(getPath());									// Get the path string and use it to delete the file.
		popItem();													// POp the name we added to the path back off.
		refreshChildList();										// Refresh the child list now missing the file.
		ourFileDir->refresh();									// Refresh the dialog box's file list.
	}
}


void	fSaveObj::deleteFileAlert(void) {
	
	char buff[60];
	
	strcpy(buff,"Are you sure you want to delete ");
	strcat(buff,staticThis->currentItem->ourName);
	strcat(buff,"?");
	staticThis->ourAlert = new alertObj(buff,staticThis,warnAlert,true,true);
	staticThis->addObj(staticThis->ourAlert);
	staticThis->setMode(deletingFile);
}


void fSaveObj::setMode(saveModes newMode) {
	
	if (wereDoing==newFolder && newMode==choosing) {
		aKbd->setEditField(nameStr);
		nameStr->beginEditing();
	} else if (wereDoing==choosing && newMode==newFolder) {
		//nameStr->endEditing();
		//aKbd->setEditField(ourAlert->nameStr);
		//ourAlert->nameStr->beginEditing();
	}
	wereDoing = newMode;
}


void  fSaveObj::handleCom(stdComs comID) {

	switch(comID) {
		case okCmd				:
			if (wereDoing==newFolder) {
				//createFolder(folderName);
				delete(ourAlert);
				ourAlert = NULL;
				setMode(choosing);
				ourPanel->setNeedRefresh();
			} else if (wereDoing==deletingFile) {
				deleteFile();
				delete(ourAlert);
				ourAlert = NULL;
				setName("noName.bmp");
				setMode(choosing);
				ourPanel->setNeedRefresh();
			} else {
				fileBaseViewer::handleCom(comID);
			}		
		break;
		case cancelCmd			:
			if (wereDoing==newFolder) {
				delete(ourAlert);
				ourAlert = NULL;
				setMode(choosing);
				ourPanel->setNeedRefresh();
			} else if (wereDoing==deletingFile) {
				delete(ourAlert);
				ourAlert = NULL;
				setMode(choosing);
				ourPanel->setNeedRefresh();
			} else {
				fileBaseViewer::handleCom(comID);
			}
		break;
		case newFolderCmd		:	
			drawNotify.setCallback(fSaveObj::newFolderAlert);
		break;
		case deleteItemCmd	:
			drawNotify.setCallback(fSaveObj::deleteFileAlert);
		break;
		default 					:	fileBaseViewer::handleCom(comID);	break;
	}
}