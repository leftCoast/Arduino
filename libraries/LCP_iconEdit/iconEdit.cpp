#include <iconEdit.h>

#include <debug.h>

#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE


// For now we do this. Later we'll have more info.
bool iconEditFilter(const char* fileName) { return appleFilter(fileName); }



// And it all starts up again..
iconEdit::iconEdit(lilOS* ourOS,int ourAppID)
	: documentPanel(ourAppID,DEF_FLDR) { }


// The world as you know it, is ending..
iconEdit::~iconEdit(void) { }


// This sets up a path for a new document. We override the original so that they will all
// get made with OUR extension.
//bool iconEdit::setNewDocPath(const char* extension) { documentPanel::setNewDocPath(".bmp"); }


// This creates the docFileObj that's used as the manager for whatever file we are
// editing. It is declared as a pure virtual, so we can/must use it to make OUR kind of
// document.
void iconEdit::createDocObj(void) { 
ST
	ourDoc = new iconEditScr(EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,docFolder);
}


// setup() & loop() panel style.
void iconEdit::setup(void) {
ST
	documentPanel::setup();
	if (ourDoc) {
		addObj((iconEditScr*)ourDoc);
	}
}



// The default here is to not draw ourselves. You can change that.
void iconEdit::drawSelf(void) {
ST
	colorObj	aColor;
	rect		aRect;
	
	aRect.setRect(this);						// Lets grab our rectangle.
	aRect.y = aRect.y + 22;					// For menu..
	aRect.height = aRect.height - 22;	// For menu..
	aColor.setColor(LC_GREY);				// Make it a nice color.
	screen->fillRect(&aRect,&aColor);	// Draw it!
}



void  iconEdit::handleCom(stdComs comID) { documentPanel::handleCom(comID); }


