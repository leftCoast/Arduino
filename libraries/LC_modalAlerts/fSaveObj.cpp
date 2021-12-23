
#include <fSaveObj.h>
#include <resizeBuff.h>
#include <bmpKeyboard.h>
#include <editLabel.h>
#include <colorRect.h>

#include <debug.h>

// The overall size and placement of the save d-box.
#define SAVE_X			30
#define SAVE_Y			30
#define SAVE_W			160
#define SAVE_H			155

// And the label for the top of the d-box
#define LABEL_X		5
#define LABEL_Y		11
#define LABEL_W		SAVE_W - LABEL_X*2
#define LABEL_H		18

// The directory popup stuff.
#define DIR_X	10
#define DIR_Y	24

// File list, size & num items.
#define FILE_LIST_X	10
#define FILE_LIST_Y	DIR_Y + 24

// The editable file name.
#define NAME_STR_X	DIR_X
#define NAME_STR_Y	143
#define NAME_STR_W	100
#define NAME_STR_H	18

// New folder button.
#define NEW_FLDR_X	120	
#define NEW_FLDR_Y	FILE_LIST_Y

#define DELETE_X		NEW_FLDR_X
#define DELETE_Y		NEW_FLDR_Y + 45



// **************************************************************
// ******************* folderAlert stuff ************************
// **************************************************************


// folderAlert::folderAlert(char* msg,saveKbd* inKbd,listener* inListener)
// 	: alertObj(msg,inListener,choiceAlert,false,false) { 
// 	
// 	colorRect*	aRect;
// 	colorObj		backColor;
// 	
// 	sKbd = inKbd;
// 	backColor.setColor(LC_GREY);
// 	aRect = new colorRect(NEW_NAME_X-2,NEW_NAME_Y-2,NEW_NAME_W+4,NEW_NAME_H+4,1);
// 	aRect->setColor(&backColor);
// 	addObj(aRect);
// 	folderPath = new editLabel(NEW_NAME_X,NEW_NAME_Y,NEW_NAME_W,NEW_NAME_H,"newName");
// 	folderPath->setColors(&black,&backColor);
// 	sKbd->setEditField(folderPath);
// 	folderPath->beginEditing();
// 	addObj(folderPath);
// }
// 	
// 	
// folderAlert::~folderAlert(void) {  }
// 
// 
// //Is this event for us? Yes, they ALL are.	
// bool folderAlert::acceptEvent(event* inEvent,point* locaPt) {
// 
// 	int	globalY;
// 	
// 	globalY = screen->gY(inEvent->mTouchPos.y);		// Event comes in a local point, get global.
// 	Serial.println(inEvent->mTouchPos.y);
// 	Serial.println(globalY);
// 	Serial.println(sKbd->y);
// 	Serial.println("-----");
// 	if (globalY-45<sKbd->y) {									// If the y point is above the keyboard..
// 		return modal::acceptEvent(inEvent,locaPt);	// We do the "modal" thing.
// 	}
// 	return false;												// Else, we pass it on for the keyboard to catch.								
// }


// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************


	
fSaveObj::fSaveObj(listener* inListener,bool(*funct)(char*))
	:fileViewer(inListener,funct),
	kbdUser() {
	
	stdComBtn*	folderBtn;
	stdComBtn*	delBtn;
	
	kbdUser::setListener(this);
	savePath				= NULL;
	wereDoing			= choosing;
	this->setRect(SAVE_X,SAVE_Y,SAVE_W,SAVE_H);											// Resize our window.
	theMsg->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);								// Resize the label to fit.
	theMsg->setText("  Choose save location");											// Set the label name.
	if (okBtn) {																						// If we have a default success button..
		delete(okBtn);																				// Delete it.
		okBtn = NULL;																				// And NULL it out as a flag.
	}
	if (cancelBtn) {																						// If we have a default cancel button..
		delete(cancelBtn);																				// Delete it.
		cancelBtn = NULL;																				// And NULL it out as a flag.
	}
	ourListBox->setLocation(FILE_LIST_X,FILE_LIST_Y);
	ourFileDir->setLocation(DIR_X,DIR_Y);												// Hook it into the drawObject ist.
	nameStr = new editLabel(NAME_STR_X,NAME_STR_Y,NAME_STR_W,NAME_STR_H);		// Create a name string obj.
	addObj(nameStr);																				// Hook it into the drawObject ist.
	setEditField(nameStr);
	setName("noName.bmp");																		// Set the name to a default for now.
	folderBtn = newStdBtn(NEW_FLDR_X,NEW_FLDR_Y,icon32,newFolderCmd,this);
	addObj(folderBtn);
	delBtn = newStdBtn(DELETE_X,DELETE_Y,icon32,deleteItemCmd,this);
	addObj(delBtn);
}
	

fSaveObj::~fSaveObj(void) { resizeBuff(0,&savePath); }


void fSaveObj::setName(char* inName) {

	if (nameStr) {
		if (nameStr->mEditing) {
			nameStr->endEditing();
		}
		nameStr->setValue(inName);
		nameStr->beginEditing();
	}
}

// 
// char* fSaveObj::getSavePath(void) { return savePath; }
// 
// 
// void fSaveObj::setItem(fileListItem* currentSelected) {
// 
// 	fileBaseViewer::setItem(currentSelected);
// 	if (currentSelected->ourType==fileType) {
// 		nameStr->endEditing();
// 		nameStr->setValue(currentSelected->ourName);
// 		nameStr->beginEditing();
// 	}
// }
// 

// void	fSaveObj::newFolderAlert(void) {
// 	
// 	nameStr->endEditing();
// 	newFldrAlertPtr = new folderAlert("Enter new folder name.",staticThis->aKbd,staticThis);
// 	setMode(newFolder);
// }


// This deletes the selected file from the SD card.
// void	fSaveObj::createFolder(void) {
// 		
// 	char* newPathStr;
// 	int	numChars;
// 	
// 	newPathStr = NULL;														// Starting off at NULL.
// 	numChars = newFldrAlertPtr->folderPath->getNumChars();		// Grab the number of chars in the new name. (minus '\0')
// 	numChars = numChars + numPathBytes();								// Add to this the total chars (including '\0') of our current path.
// 	if (resizeBuff(numChars,&newPathStr)) {							// If we can grab the RAM..
// 		strcpy(newPathStr,getPath());										// Stuff in our current path string.
// 		strcat(newPathStr,newFldrAlertPtr->folderPath->buff);		// Add to that the new folder name string (Can be path)
// 		SD.mkdir(newPathStr);												// USe the new path string to create the folder.
// 		resizeBuff(0,&newPathStr);											// Recycle the RAM.
// 		refreshChildList();													// Refresh the child list now missing the file.
// 		ourFileDir->refresh();												// Refresh the dialog box's file list.
// 	}
// }



// This deletes the selected file from the SD card.
// void	fSaveObj::deleteFile() {
// 
// 	if (pushChildItemByName(currentItem->ourName)) {	// Add the selected name to the end of the path.
// 		SD.remove(getPath());									// Get the path string and use it to delete the file.
// 		popItem();													// POp the name we added to the path back off.
// 		refreshChildList();										// Refresh the child list now missing the file.
// 		ourFileDir->refresh();									// Refresh the dialog box's file list.
// 	}
// }


// void	fSaveObj::deleteFileAlert(void) {
// 	
// 	char buff[60];
// 	
// 	strcpy(buff,"Are you sure you want to delete ");
// 	strcat(buff,staticThis->currentItem->ourName);
// 	strcat(buff,"?");
// 	staticThis->deleteAlert = new alertObj(buff,staticThis,warnAlert,true,true);
// 	staticThis->addObj(staticThis->deleteAlert);
// 	staticThis->setMode(deletingFile);
// }


void fSaveObj::setMode(saveModes newMode) {
	
	if (wereDoing==newFolder && newMode==choosing) {
			setEditField(nameStr);
	}
	wereDoing = newMode;
}


void fSaveObj::handleCom(stdComs comID) {

	switch(wereDoing) {
		case choosing		:
			switch(comID) {
				case newFolderCmd		:	
					//drawNotify.setCallback(fSaveObj::newFolderAlert);
				break;
				case deleteItemCmd	:
					//drawNotify.setCallback(fSaveObj::deleteFileAlert);
				break;
				default 					:
					db.trace("got a command..",comID);
					fileViewer::handleCom(comID);
				break;
			}			
		break;
		case newFolder		:
			switch (comID) {
				case okCmd		:
					//createFolder();
					setMode(choosing);
				break;
				case cancelCmd	:
					setMode(choosing);
				break;
			}
		break;
		case deletingFile	:
			switch (comID) {
				case okCmd		:
					//deleteFile();
					setName("noName.bmp");
					setMode(choosing);
				break;
				case cancelCmd	:
					setMode(choosing);
				break;
			}
		break;
	}
}
