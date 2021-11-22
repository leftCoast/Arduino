#include <fSaveObj.h>


// The overall size and placement of the save d-box.
#define SAVE_X	45
#define SAVE_Y	40
#define SAVE_W	150
#define SAVE_H	230

// Cancel & Ok Button locations.
#define CNCL_X	10
#define CNCL_Y	SAVE_H - 40
#define OK_X	SAVE_W - 32 - CNCL_X
#define OK_Y	SAVE_H - 40

// And the label for the top of the d-box
#define LABEL_X	5
#define LABEL_Y	10
#define LABEL_W	SAVE_W - LABEL_X * 2
#define LABEL_H	18

#define PATH_STR_X	5
#define PATH_STR_Y	SAVE_H - 80
#define PATH_STR_W	SAVE_W - 2*PATH_STR_X
#define PATH_STR_H	18

#define NAME_STR_X	5
#define NAME_STR_Y	PATH_STR_Y + 20
#define NAME_STR_W	100
#define NAME_STR_H	18




	
fSaveObj::fSaveObj(panel* inPanel,bool(*funct)(char*))
	:fileBaseViewer(inPanel,funct) {
	
	this->setRect(SAVE_X,SAVE_Y,SAVE_W,SAVE_H);
	ourLabel->setRect(LABEL_X,LABEL_Y,LABEL_W,LABEL_H);
	ourLabel->setValue("Choose save location.");
	sBtn->setLocation(OK_X,OK_Y);
	cBtn->setLocation(CNCL_X,CNCL_Y);
	pathStr = new label(PATH_STR_X,PATH_STR_Y,PATH_STR_W,PATH_STR_H);
	addObj(pathStr);
	nameStr = new label(NAME_STR_X,NAME_STR_Y,NAME_STR_W,NAME_STR_H);
	addObj(nameStr);
	setName("starTrek.bmp");
	showPath();
}
	
	
fSaveObj::~fSaveObj(void) {  }


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

	fileBaseViewer::setSuccess(trueFalse);

}
