#include  <iconEdit.h>


#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE

#define TEST_PATH "/cut32.bmp"// /iconEdit/test.bmp"



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
	
  	openDBox = NULL;
  	saveDBox = NULL;
	if (!SD.exists(TEST_PATH)) {
		createNewBmpFile(TEST_PATH,32,32);
	}
}


// The world as you know it, is ending..
iconEdit::~iconEdit(void) { }


// Open up the file choosing dialog box.
void iconEdit::beginFileOpen(void) {

	openDBox = new fOpenObj(this);
	addObj(openDBox);
}


// Open up the file saving dialog box.
void iconEdit::beginFileSave(void) {

	saveDBox = new fSaveObj(this);
	addObj(saveDBox);
}


// setup() & loop() panel style.
void iconEdit::setup(void) {
	
	theEditScr = new iconEditScr(mOSPtr,EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,TEST_PATH);
	addObj(theEditScr);
	
	openFileBtn* ourOpenBtn = new openFileBtn(30,1,mOSPtr->stdIconPath(fOpen22),this);
	mMenuBar->addObj(ourOpenBtn);
	
	saveFileBtn* ourSaveBtn = new saveFileBtn(60,1,mOSPtr->stdIconPath(fSave22),this);
	mMenuBar->addObj(ourSaveBtn);
}


void iconEdit::loop(void) {
	
	if (openDBox) {
		if (openDBox->taskComplete()) {
			delete(openDBox);
			openDBox = NULL;
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
