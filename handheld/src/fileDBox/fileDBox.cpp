#include  "fileDBox.h"

panel*	fileDBCaller	= NULL;
char*		fileDBInitPath	= NULL;
char*		fileDBResult	= NULL;

// ***************************************************************
//                            fileOpen
// ***************************************************************


// And it all starts up again..
fileOpen::fileOpen(void)
  : panel(fileOpenApp) { }


// The world as you know it, is ending..
fileOpen::~fileOpen(void) { }


// setup() & loop() panel style.
void fileOpen::setup(void) {
		
	screen->fillScreen(&green);
}


void fileOpen::loop(void) {

}


// The default here is to not draw ourselves. You can change that.
void fileOpen::drawSelf(void) { }


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void fileOpen::closing(void) {  }



// ***************************************************************
//                            fileSave
// ***************************************************************


// And it all starts up again..
fileSave::fileSave(void)
  : panel(fileSaveApp) { }


// The world as you know it, is ending..
fileSave::~fileSave(void) { }


// setup() & loop() panel style.
void fileSave::setup(void) {
		
	screen->fillScreen(&green);
}


void fileSave::loop(void) {

}


// The default here is to not draw ourselves. You can change that.
void fileSave::drawSelf(void) { }


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void fileSave::closing(void) {  }
