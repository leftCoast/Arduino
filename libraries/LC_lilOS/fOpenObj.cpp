#include <fOpenObj.h>


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


fOpenObj::fOpenObj(panel* inPanel,bool(*funct)(char*))
	:fileBaseViewer(inPanel,funct) {
	
	this->setRect(OPEN_X,OPEN_Y,OPEN_W,OPEN_H);
	ourLabel->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);
	ourLabel->setValue("Choose file");
	sBtn->setLocation(OK_X,OK_Y);
	cBtn->setLocation(CNCL_X,CNCL_Y);
}


fOpenObj::~fOpenObj(void) { }


// Someone double clicked on a file. Open this one!
void fOpenObj::chooseFile(char* name) {
	
	currentItem = NULL;				// On a double click we already have the name. Clear this.
	pushChildItemByName(name);		// Push the clicked name into the path.
	setSuccess(true);					// And we call success to end this.
}


// The file viewer is going down either with a success or a failure. Deal with that here.
void fOpenObj::setSuccess(bool trueFalse) {

	if (currentItem && trueFalse) {							// If we have a current item and succes will be true..
		if (currentItem->ourType==fileType) {				// If this item is a file..
			pushChildItemByName(currentItem->ourName);	// Push it onto the path.
		}
	}
	fileBaseViewer::setSuccess(trueFalse);					// Call the inherited to unwind everthing.
}



