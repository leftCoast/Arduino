#include  <iconEdit.h>
#include <fOpenObj.h>


#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE

#define TEST_PATH "/cut32.bmp"// /iconEdit/test.bmp"


saveBtn::saveBtn(int xLoc, int yLoc,char* path,iconEditScr* editScr)
	:iconButton22(xLoc,yLoc,path) { theEditScr =  editScr; }
	
	
saveBtn::~saveBtn(void) {  }				

void saveBtn::doAction(void) {
	Serial.println("saving file clicked.");
	theEditScr->saveDocFile();
}
	



// And it all starts up again..
iconEdit::iconEdit(lilOS* ourOS,int ourAppID)
	: panel(ourOS,ourAppID) {
  
	if (!SD.exists(TEST_PATH)) {
		createNewBmpFile(TEST_PATH,32,32);
	}
}


// The world as you know it, is ending..
iconEdit::~iconEdit(void) { }


// setup() & loop() panel style.
void iconEdit::setup(void) {
	
	theEditScr = new iconEditScr(mOSPtr,EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,TEST_PATH);
	addObj(theEditScr);
	
	saveBtn* ourSaveBtn = new saveBtn(30,1,mOSPtr->stdIconPath(fSave22),theEditScr);
	mMenuBar->addObj(ourSaveBtn);
	
	fOpenObj*  theObject = new fOpenObj();
	addObj(theObject);
}


void iconEdit::loop(void) {

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
