#include  <iconEdit.h>


#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE



// And it all starts up again..
iconEdit::iconEdit(lilOS* ourOS,int ourAppID)
	: panel(ourAppID) {
	
  	openDBox		= NULL;
  	saveDBox		= NULL;
  	theEditScr	= NULL;
}


// The world as you know it, is ending..
iconEdit::~iconEdit(void) { }


bool myFilter(char* fileName) {

	if (fileName[0]=='_') 					return false;
	if (!strcmp(fileName,"SPOTLI~1"))	return false;
	if (!strcmp(fileName,"TRASHE~1"))	return false;
	if (strstr(fileName,"DS_STO"))		return false;
	if (strstr(fileName,".DS_"))			return false;
	else return true;
}


// Start a fresh new file to edit.
void iconEdit::beginFileNew(void) {

	// Check if there are edits to deal with.
	// Of everything is ok, create the new document with defualt name.
}


// Open up the file choosing dialog box.
void iconEdit::beginFileOpen(void) {

	openDBox = new fOpenObj(this,myFilter);
	addObj(openDBox);
}


// Open up the file saving dialog box.
void iconEdit::beginFileSave(void) {

	saveDBox = new fSaveObj(this,myFilter);
	addObj(saveDBox);
}


// setup() & loop() panel style.
void iconEdit::setup(void) {
	
	stdComBtn* ourNewBtn = newStdIcon(40,1,icon22,newFileCmd,this);
	mMenuBar->addObj(ourNewBtn);
	stdComBtn* ourOpenBtn = newStdIcon(80,1,icon22,openFileCmd,this);
	mMenuBar->addObj(ourOpenBtn);
	stdComBtn* ourSaveBtn = newStdIcon(120,1,icon22,saveFileCmd,this);
	mMenuBar->addObj(ourSaveBtn);
}


void iconEdit::loop(void) {
	
	if (openDBox) {
		if (openDBox->done) {
			if (openDBox->success) {
				if (theEditScr) {
					delete theEditScr;
				}
				Serial.println(openDBox->getPath());
				theEditScr = new iconEditScr(EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,openDBox->getPath());
				addObj(theEditScr);
			}
			delete(openDBox);
			openDBox = NULL;
			setNeedRefresh(true);
		}
	}
	if (saveDBox) {
		if (saveDBox->done) {
			if (saveDBox->success && theEditScr) {					
				theEditScr->saveDocFile(saveDBox->getSavePath());
			}
			delete(saveDBox);
			saveDBox = NULL;
			setNeedRefresh(true);
		}
	}
}


// The default here is to not draw ourselves. You can change that.
void iconEdit::drawSelf(void) {

	colorObj	aColor;
	
	aColor.setColor(LC_GREY);
	screen->fillRect(this,&aColor);
}


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void iconEdit::closing(void) { theEditScr->closeDocFile(); }



void  iconEdit::handleCom(stdComs comID) {

	switch(comID) {
		case newFileCmd	: beginFileNew();				break;
		case openFileCmd	: beginFileOpen();			break;
		case saveFileCmd	: beginFileSave();			break;
		default				: panel::handleCom(comID);	break;
	}
}


