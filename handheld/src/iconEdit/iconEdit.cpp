#include  "iconEdit.h"


// And it all starts up again..
iconEdit::iconEdit()
  : panel(iconEditApp) { }


// The world as you know it, is ending..
iconEdit::~iconEdit(void) { }


// setup() & loop() panel style.
void iconEdit::setup(void) {
		
	screen->fillScreen(&white);
	
}


void iconEdit::loop(void) {

}


// The default here is to not draw ourselves. You can change that.
void iconEdit::drawSelf(void) { }


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void iconEdit::closing(void) {  }
