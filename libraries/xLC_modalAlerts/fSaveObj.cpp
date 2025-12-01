
#include <fSaveObj.h>
#include <resizeBuff.h>
#include <bmpKeyboard.h>
#include <editLabel.h>
#include <colorRect.h>

//#include <debug.h>

// The overall size and placement of the save d-box.
#define SAVE_X			30
#define SAVE_Y			28
#define SAVE_W			160
#define SAVE_H			156

// And the label for the top of the d-box
#define LABEL_X		5
#define LABEL_Y		8
#define LABEL_W		SAVE_W - LABEL_X*2
#define LABEL_H		18

// The directory popup stuff.
#define DIR_X	10
#define DIR_Y	20
#define DIR_W	100
#define DIR_H	18

// File list, size & num items.
#define FILE_LIST_X	10
#define FILE_LIST_Y	DIR_Y + 23

// The editable file name.
#define NAME_STR_X	DIR_X
#define NAME_STR_Y	141
#define NAME_STR_W	100
#define NAME_STR_H	18

// New folder button.
#define NEW_FLDR_X	120	
#define NEW_FLDR_Y	FILE_LIST_Y

// Delete file/folder button.
#define DELETE_X		NEW_FLDR_X
#define DELETE_Y		NEW_FLDR_Y + 45

#define NEW_NAME_X	50
#define NEW_NAME_Y	45
#define NEW_NAME_W	100
#define NEW_NAME_H	10

#define NEW_FLDR_MSG	"Enter new folder name."
#define DELETE_MSG	"Are you sure you want to delete "

// **************************************************************
// ******************* deleteAlert stuff ************************
// **************************************************************


deleteAlert::deleteAlert(listener* inListener,filePath* inPath,char* selectName)
	: alertObj("",inListener,warnAlert,false,false),
	kbdUser(this) {
	
	int			numBytes;
	char* 		resultMsg;
	pathItem*	currItem;
	
	ourPath			= inPath;
	resultMsg		= NULL;
	ourSelectName	= selectName;									// We're going to trust that this is good for the duration.
	if (selectName) {
		ourPath->pushChildItemByName((char*)selectName);
	}
	currItem = inPath->getCurrItem();
	if (currItem) {
		numBytes = 30 + strlen(DELETE_MSG);
		if (resizeBuff(numBytes,&resultMsg)) {
			strcpy(resultMsg,DELETE_MSG);
			if (currItem->getType()==folderType) {
				strcat(resultMsg,"folder:");
				strcat(resultMsg,currItem->getName());
				setMsg(resultMsg);
			} else if (currItem->getType()==fileType) {
				strcat(resultMsg,"file:");
				strcat(resultMsg,currItem->getName());
				setMsg(resultMsg);
			} else {
				setMsg("Whatever! this is messed up!");
			}
			resizeBuff(0,&resultMsg);
		}
	}
}
	

// Your standard destructor.	
deleteAlert::~deleteAlert(void) { }


// Is this event for us? Well yes, they ALL are. Except if we have a keyboard..	
bool deleteAlert::acceptEvent(event* inEvent,point* locaPt) {

	point	globalPt;
	
	if (drawGroup::acceptEvent(inEvent,locaPt)) {	// If its actually ours..
		return true;	 										// We return true.
	}																//
	globalPt = screen->gP(locaPt);						// Modals are global. So global is good.
	checkKbdEvent(inEvent,&globalPt);					// Not ours? Let the keyboard have a shot at it.
	return true;												// In all cases, the buck stops here.
}


// Its time to actually delete the item from whatever the user gave us. Either it works
// or it doesn't. We don't check in this routine. 
void deleteAlert::deleteItem(void) {

	ourPath->deleteCurrentItem();		// Kill off the offending item..
	ourPath->refreshChildList();		// Redo the child list without the item.
}


// Clicks and commands come filtering in here..
void deleteAlert::handleCom(stdComs comID) {
	
	switch(comID) {						// Check what we got as a command..
		case okCmd		:					// We got an ok click..
			deleteItem();					// Have a shot at deleting the item.
			done = true;					// This alert is done!
		break;								// Zoop! We're gone!
		case cancelCmd	:					// Both okCmd & cancelCmd land here.
			if (ourSelectName) {			// If we had a name to add..
				ourPath->popItem();		// We need to loose it.
			}									//
			done = true;					// This alert is done!
		break;								// And these case statements are done!
		default			: break;			// Just to shut up the compiler.
	}											//
	ourListener->handleCom(comID);	// Let our listener have a shot.		
}



// **************************************************************
// ******************* folderAlert stuff ************************
// **************************************************************


folderAlert::folderAlert(listener* inListener,filePath* inPath)
	: alertObj(NEW_FLDR_MSG,inListener,choiceAlert,false,false),
	kbdUser(this) { 
	
	colorRect*	aRect;
	colorObj		backColor;
	
	ourPath = inPath;
	backColor.setColor(LC_GREY);
	aRect = new colorRect(NEW_NAME_X-2,NEW_NAME_Y-2,NEW_NAME_W+4,NEW_NAME_H+4,1);
	aRect->setColor(&backColor);
	addObj(aRect);
	nameLabel = new editLabel(NEW_NAME_X,NEW_NAME_Y,NEW_NAME_W,NEW_NAME_H,"untitled");
	addObj(nameLabel);
	nameLabel->setColors(&black,&backColor);
	nameLabel->beginEditing();
	setEditField(nameLabel);
}
	

// Your standard destructor.	
folderAlert::~folderAlert(void) { }



// Is this event for us? Well yes, they ALL are. Except if we have a keyboard..	
bool folderAlert::acceptEvent(event* inEvent,point* locaPt) {

	point	globalPt;
	
	if (drawGroup::acceptEvent(inEvent,locaPt)) {	// If its actually ours..
		return true;	 										// We return true.
	}																//
	globalPt = screen->gP(locaPt);						// Modals are all global. At least the keyboard is. So global is good.
	checkKbdEvent(inEvent,&globalPt);					// Not ours? Let the keyboard have a shot at it.
	return true;												// In all cases, the buck stops here.
}


// Its time to actually create the folder from whatever the user gave us. Either it works
// or it doesn't. We don't check in this routine. 
void folderAlert::createFolder(void) {
	
	char* finalPath;
	int	numBytes;
	
	finalPath = NULL;														// All pointers we start at NULL.
	if (ourPath) {															// If we actually have a path.. (sanity)
		numBytes = ourPath->numPathBytes();							// Get the amount of bytes the path string needs.
		numBytes = numBytes + strlen(nameLabel->editBuff);		// Add the number of bytes the name string needs.
		if(resizeBuff(numBytes,&finalPath)) {						// If we can grab the memory for this..
			strcpy(finalPath,ourPath->getPath());					// Our original path goes in.
			strcat(finalPath,nameLabel->editBuff);					// We add the user's name/path to it.
			SD.mkdir(finalPath);											// Tell the SD card lib to create the folder.
			resizeBuff(0,&finalPath);									// Recycle the path memory.
			ourPath->refreshChildList();								// Get our new folder into the child list.
		}
	}
}


// Clicks and commands come filtering in here..
void folderAlert::handleCom(stdComs comID) {
	
	switch(comID) {						// Check what we got as a command..
		case okCmd		:					// We got an ok click..
			createFolder();				// Have a shot at creating a folder.
		case cancelCmd	:					// Both okCmd & cancelCmd land here.
			done = true;					// This alert is done!
		break;								// And these case statements are done!
		default			: break;			// Error, pass through I guess..
	}											//
	ourListener->handleCom(comID);	// Let our listener have a shot.		
}



// **************************************************************
// *********************** saveFileDir **************************
// **************************************************************


saveFileDir::saveFileDir(int inX, int inY, int inWidth,int inHeight,fSaveObj* inViewer,fileListBox* inListBox)
	:fileDir(inX,inY,inWidth,inHeight,inViewer,inListBox) { finalPath	= NULL; }
	
	
saveFileDir::~saveFileDir(void) { if (finalPath) resizeBuff(0,&finalPath); }


// This is what is called when we receive a click from the user..
void saveFileDir::doAction(void) {
	
	((fSaveObj*)ourViewer)->setSelectedName(NULL);	// Clear out the saved selected name.
	fileDir::doAction();										// Do whatever you need to do.
}


// When doing a save of a file. The thing we're looking for is a default file name. This
// is what we're going to use setItem() for. If the user clicks a file, we will update its
// editable name field with that name.
void saveFileDir::setItem(pathItemType inType,const char* name) {

	((fSaveObj*)ourViewer)->setSelectedName(name);	// We let them all be saved for later.
	if (inType == fileType) {								// If its a file..
		((fSaveObj*)ourViewer)->setName(name);			// Its used to set the editable file name.
	}
}


// Afer all the smoke and nonsense is over. This is the resulting choice from the user.
// It is the current path + the editable name from ourViewer.
char* saveFileDir::endChoice(void) {
	
	int	numBytes;
	
	numBytes = numPathBytes();
	numBytes = numBytes + strlen(((fSaveObj*)ourViewer)->getName());
	if(resizeBuff(numBytes,&finalPath)) {
		strcpy(finalPath,getPath());
		strcat(finalPath,((fSaveObj*)ourViewer)->getName());
	}
	return finalPath;
}



// **************************************************************
// ********************* fSaveObj stuff *************************
// **************************************************************


	
fSaveObj::fSaveObj(listener* inListener,bool(*funct)(const char*))
	:fileViewer(inListener,funct),
	kbdUser(this) {
	
	stdComBtn*	folderBtn;
	stdComBtn*	delBtn;
	
	savePath				= NULL;
	selectedName		= NULL;
	wereDoing			= choosing;
	this->setRect(SAVE_X,SAVE_Y,SAVE_W,SAVE_H);										// Resize our window.
	theMsg->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);								// Resize the label to fit.
	theMsg->setText("   Choose save location");										// Set the label name.
	if (okBtn) {																				// If we have a default success button..
		delete(okBtn);																			// Delete it.
		okBtn = NULL;																			// And NULL it out as a flag.
	}
	if (cancelBtn) {																			// If we have a default cancel button..
		delete(cancelBtn);																	// Delete it.
		cancelBtn = NULL;																		// And NULL it out as a flag.
	}
	ourListBox->setLocation(FILE_LIST_X,FILE_LIST_Y);								// Move the list box to where we want it.
	ourFileDir->setLocation(DIR_X,DIR_Y);												// Hook it into the drawObject ist.
	nameLabel = new editLabel(NAME_STR_X,NAME_STR_Y,NAME_STR_W,NAME_STR_H);	// Create a name string obj.
	nameLabel->setEventSet(touchLift);
	addObj(nameLabel);																			// Hook it into the drawObject ist.
	setEditField(nameLabel);
	setName("untitled.bmp");																	// Set the name to a default for now.
	folderBtn = newStdBtn(NEW_FLDR_X,NEW_FLDR_Y,icon32,newFolderCmd,this);
	addObj(folderBtn);
	delBtn = newStdBtn(DELETE_X,DELETE_Y,icon32,deleteItemCmd,this);
	addObj(delBtn);
	if (ourFileDir) delete(ourFileDir);													// Loose the original.
	ourFileDir = new saveFileDir(DIR_X,DIR_Y,DIR_W,DIR_H,this,ourListBox);	// Create our version.
	if (ourFileDir) {																			// If everything went ok..
		addObj(ourFileDir);																	// Add our new one to the draw list.
		ourFileDir->setPath("/");															// Set some sort of default path.
		ourFileDir->refresh();
	}
}


fSaveObj::~fSaveObj(void) { 

	resizeBuff(0,&savePath);
	resizeBuff(0,&selectedName);
}


char*	fSaveObj::getName(void) { return nameLabel->editBuff; }


void fSaveObj::setName(const char* inName) {

	if (nameLabel) {
		if (nameLabel->mEditing) {
			nameLabel->endEditing();
		}
		nameLabel->setValue((char*)inName);
		nameLabel->beginEditing();
	}
}

// We're going to keep a copy of the current selected name off the child list.
void	fSaveObj::setSelectedName(const char* name) {
	
	if (!name) {															// If we got a NULL..
		resizeBuff(0,&selectedName);									// Recycle the buffer, set to NULL.
	} else if (resizeBuff(strlen(name)+1,&selectedName)) {	// Else, if we can allocate the new buffer..
		strcpy(selectedName,name);										// We copy the name for later.
	}	
}


void fSaveObj::setMode(saveModes newMode) {
	
	switch(wereDoing) {
		case choosing		:
			switch(newMode) {
				case choosing		: return;
				case newFolder		:
					newFldrAlert = new folderAlert(this,ourFileDir);
				break;
				case deletingFile	:
					doDeleteAlert  = new deleteAlert(this,ourFileDir,selectedName);
				break;
			}
		break;
		case newFolder		:
			switch(newMode) {
				case choosing		:
					drawing = false;
					ourFileDir->refresh();
				break;
				case newFolder		: return;
				case deletingFile	:
					doDeleteAlert  = new deleteAlert(this,ourFileDir,selectedName);
				break;
			}
		break;
		case deletingFile	:
			switch(newMode) {
				case choosing		:
					drawing = false;
					ourFileDir->refresh();
				break;
				case newFolder		:
					newFldrAlert = new folderAlert(this,ourFileDir);
				break;
				case deletingFile	: return;
			}
		break;
	}
	wereDoing = newMode;
}


void fSaveObj::handleCom(stdComs comID) {

	switch(comID) {
		case newFolderCmd		: setMode(newFolder);				break;
		case deleteItemCmd	: setMode(deletingFile);			break;
		case okCmd				:
		case cancelCmd			:
			if (wereDoing==choosing) {
				fileViewer::handleCom(comID);
			} else {
				setMode(choosing);
			}
		break;
		default 					: fileViewer::handleCom(comID);	break;
	}			
}
