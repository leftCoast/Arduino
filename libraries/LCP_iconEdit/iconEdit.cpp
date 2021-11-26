#include  <iconEdit.h>


#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE

// **************************************************************
// ********************** newFileBtn stuff **********************
// **************************************************************


newFileBtn::newFileBtn(int xLoc, int yLoc,char* path,iconEdit* inApp)
	:iconButton22(xLoc,yLoc,path) { ourApp =  inApp; }
	
	
newFileBtn::~newFileBtn(void) {  }				

void newFileBtn::doAction(void) {
	
	ourApp->mOSPtr->beep();
	ourApp->beginFileNew();
}
	

	
// **************************************************************
// ********************* saveFileBtn  stuff *********************
// **************************************************************


openFileBtn::openFileBtn(int xLoc, int yLoc,char* path,iconEdit* inApp)
	:iconButton22(xLoc,yLoc,path) { ourApp =  inApp; }
	
	
openFileBtn::~openFileBtn(void) {  }				

void openFileBtn::doAction(void) {
	
	ourApp->mOSPtr->beep();
	ourApp->beginFileOpen();
}
	

	
// **************************************************************
// ********************* saveFileBtn  stuff *********************
// **************************************************************


saveFileBtn::saveFileBtn(int xLoc, int yLoc,char* path,iconEdit* inApp)
	:iconButton22(xLoc,yLoc,path) { ourApp =  inApp; }
	
	
saveFileBtn::~saveFileBtn(void) {  }				


void saveFileBtn::doAction(void) {
	
	ourApp->mOSPtr->beep();
	ourApp->beginFileSave();
}
	
	
	
// **************************************************************
// *********************** iconEdit stuff ***********************
// **************************************************************


// And it all starts up again..
iconEdit::iconEdit(lilOS* ourOS,int ourAppID)
	: panel(ourOS,ourAppID) {
	
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
	
	//theEditScr = new iconEditScr(mOSPtr,EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,TEST_PATH);
	//addObj(theEditScr);
	
	newFileBtn* ourNewBtn = new newFileBtn(40,1,mOSPtr->stdIconPath(fNew22),this);
	mMenuBar->addObj(ourNewBtn);
	
	openFileBtn* ourOpenBtn = new openFileBtn(80,1,mOSPtr->stdIconPath(fOpen22),this);
	mMenuBar->addObj(ourOpenBtn);
	
	saveFileBtn* ourSaveBtn = new saveFileBtn(120,1,mOSPtr->stdIconPath(fSave22),this);
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
				theEditScr = new iconEditScr(mOSPtr,EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,openDBox->getPath());
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
