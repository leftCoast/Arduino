#include <fSaveObj.h>
#include <resizeBuff.h>
#include <bmpKeyboard.h>
#include <editLabel.h>

#define out(x) Serial.print(x)
#define outln(x) Serial.println(x)

// The overall size and placement of the save d-box.
#define SAVE_X	0
#define SAVE_Y	24
#define SAVE_W	240
#define SAVE_H	320 - SAVE_Y


// And the label for the top of the d-box
#define LABEL_X	5
#define LABEL_Y	10
#define LABEL_W	SAVE_W - LABEL_X * 2
#define LABEL_H	18

#define PATH_STR_X	5
#define PATH_STR_Y	145
#define PATH_STR_W	240 - 2*PATH_STR_X
#define PATH_STR_H	18

#define NAME_STR_X	5
#define NAME_STR_Y	150
#define NAME_STR_W	240 - 2*NAME_STR_X
#define NAME_STR_H	18



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
	this->setRect(SAVE_X,SAVE_Y,SAVE_W,SAVE_H);										// Resize our window.
	ourLabel->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);							// Resize the label to fit.
	ourLabel->setValue("Choose save location.");										// Set the label name.
	if (sBtn) {																					// If we have a default success button..
		delete(sBtn);																			// Delete it.
		sBtn = NULL;																			// And NULL it out as a flag.
	}
	if (cBtn) {																					// If we have a default cancel button..
		delete(cBtn);																			// Delete it.
		cBtn = NULL;																			// And NULL it out as a flag.
	}
	pathStr = NULL;
	//pathStr = new label(PATH_STR_X,PATH_STR_Y,PATH_STR_W,PATH_STR_H);			// create the path viewing object.
	//addObj(pathStr);																			// Hook it into the drawObject ist.
	nameStr = new editLabel(NAME_STR_X,NAME_STR_Y,NAME_STR_W,NAME_STR_H);	// Create a name string obj.
	addObj(nameStr);																			// Hook it into the drawObject ist.
	setName("starTrek.bmp");																// Set the name to a default for now.
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



