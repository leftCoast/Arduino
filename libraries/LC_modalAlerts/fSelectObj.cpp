#include <fSelectObj.h>
#include <debug.h>

// The overall size and placement of the open d-box.
#define OPEN_X	50
#define OPEN_Y	40
#define OPEN_W	140
#define OPEN_H	190

// Cancel & Ok Button locations.
#define CNCL_X	10
#define CNCL_Y	OPEN_H - 40
#define OK_X	OPEN_W - 32 - CNCL_X
#define OK_Y	OPEN_H - 40

// And the label for the top of the d-box
#define LABEL_X	5
#define LABEL_Y	10
#define LABEL_W	110   // OPEN_W - LABEL_X * 2
#define LABEL_H	18


// File list, size & num items.
#define FILE_LIST_X			10
#define FILE_LIST_Y			50
#define FILE_LIST_W			100
#define NUM_LIST_ITEMS		5


// The directory popup stuff.
#define DIR_X	FILE_LIST_X
#define DIR_Y	FILE_LIST_Y - 25
#define DIR_W	FILE_LIST_W
#define DIR_H	18


// **************************************************************
// ********************* selectFileDir stuff **********************
// **************************************************************

selectFileDir::selectFileDir(int inX, int inY, int inWidth,int inHeight,selectType inSelectType,fileViewer* inViewer,fileListBox* inListBox)
	:fileDir(inX,inY,inWidth,inHeight,inViewer,inListBox) {
	
	ourSelectType	= inSelectType;
	currentType		= noType;
	currentNamePtr	= NULL;
}
	
	
selectFileDir::~selectFileDir(void) {  }


// In this case a double click on a filename means this is the user's choice.
void selectFileDir::chooseFile(char* name) {

	if (ourSelectType!=foldersOnly) {		// If its Ok to choose a file..
		if (pushChildItemByName(name)) {		// If we can push this filename on the path..
			currentType = noType;				// Remove type. Because we've just used it.
			currentNamePtr = NULL;				// And remove the name pointer as well.
			ourViewer->handleCom(okCmd);		// We tell the viewer we're done.
		}
	}
}


// We store away the current selected type and name from the list items, just in case the
// user clicks the cmdOk button and we need to glue it to the end of our path string. But
// we also must take in account what the user is actually allowing. Looking for a file and
// getting back a folder will not win us many friends.
void selectFileDir::setItem(pathItemType inType,char* name) {

	switch(ourSelectType) {
		case filesOnly			:
			if (inType == fileType) {
				currentType		= inType;
				currentNamePtr = name;
			}
		break;
		case foldersOnly		:
			if (inType == folderType) {
				currentType		= inType;
				currentNamePtr = name;
			}
		break;
		case filesAndFolders	:
			currentType		= inType;
			currentNamePtr = name;
		break;
	}
}


// Afer all the smoke and nonsense is over. This is the resulting choice from the user.
char* selectFileDir::endChoice(void) {

	pathItem*	lastItem;
	
	if (currentNamePtr) {									// If there's a current name, remember, this has been vetted above.
		if (!pushChildItemByName(currentNamePtr)) {	// If we CAN'T push this name onto the path.
			return NULL;										// Something very wrong is going on. Return NULL.
		}
	}
	lastItem = getCurrItem();								// Grab the last item on the path.
	switch (ourSelectType) {								// Looking at our desired selection type..
		case filesOnly			:								// Only files..
			if (lastItem->getType()==fileType) {		// If this item is of type file..
				return getPath();								// All is good, we return the path.
			}
		break;
		case foldersOnly		:								// Looking for only directories..
			if (lastItem->getType()==folderType			// If its a folder..
				||lastItem->getType()==rootType) {		// OR a root..
				return getPath();								// All is good, we return the path.
			}
		break;
		case filesAndFolders	:								// Looking for either files or directories..
			return getPath();									/// Don't even bother looking. Return the path.
		break;
	}
	return NULL;												// If we arrived here? This is not what we were looking for. Return NULL.
}



// **************************************************************
// ********************** fSelectObj stuff ************************
// **************************************************************


fSelectObj::fSelectObj(listener* inListener,bool(*funct)(char*),selectType inSelectType)
	:fileViewer(inListener,funct) {
	ST
	currentItem = NULL;
	ourSelectType = inSelectType;
	this->setRect(OPEN_X,OPEN_Y,OPEN_W,OPEN_H);
	theMsg->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);
	theMsg->setText("Choose file");
	okBtn->setLocation(OK_X,OK_Y);
	cancelBtn->setLocation(CNCL_X,CNCL_Y);
}


fSelectObj::~fSelectObj(void) { }



void fSelectObj::makeFileDir(void) {
	ST
	ourFileDir = new selectFileDir(DIR_X,DIR_Y,DIR_W,DIR_H,ourSelectType,this,ourListBox);
	ourListBox->setFileDir(ourFileDir);
	if (ourFileDir) {
		addObj(ourFileDir);
		ourFileDir->setPath("/");
	}
}


// It's always nice to set a default path to make looking for things easier.
void fSelectObj::setInitialPath(char* inPath) {

	if (ourFileDir) {
		ourFileDir->setPath(inPath);
	}
}


// And we handle commands from our things.
void  fSelectObj::handleCom(stdComs comID) {

	switch(comID) {											// Checking the command we receive..
		case cancelCmd	: setSuccess(false); break;	// Cancel, set success to false and go.
		case okCmd		: 										// Ok, well that's what they clicked..
			if (!getPathResult()) {							// If there is no path result..
				comID = cancelCmd;							// Picked a blocked choice. Make it a cancel.
				setSuccess(false);							// And success is really false.
			} else {												// Else, they picked a good choice..
				setSuccess(true);								// Set success to true!
			}
		break;													// And we're done with checking
		default			: break;								// And with all our checks.
	}
	if (ourListener) ourListener->handleCom(comID);	// If we have a listener.. Let them have a go at the command.
}

