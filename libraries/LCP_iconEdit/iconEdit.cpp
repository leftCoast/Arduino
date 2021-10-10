#include  <iconEdit.h>
#include  <iconEditScr.h>

#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE

#define TEST_PATH "/system/icons/standard/app32.bmp"// /iconEdit/test.bmp"


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
		
	iconEditScr*	theEditScr;
	
	theEditScr = new iconEditScr(mOSPtr,EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,TEST_PATH);
	addObj(theEditScr);
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
void iconEdit::closing(void) {  }
