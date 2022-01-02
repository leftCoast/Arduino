#include <iconEdit.h>
#include <resizeBuff.h>

#include <debug.h>


// For now we do this. Later we'll have more info.
bool iconEditFilter(const char* fileName) { return appleFilter(fileName); }



// And it all starts up again..
iconEdit::iconEdit(lilOS* ourOS,int ourAppID)
	: documentPanel(ourAppID) { ST haveComToPassOn = false; }


// The world as you know it, is ending..
iconEdit::~iconEdit(void) { ST }


// This creates the docFileObj that's used as the manager for whatever file we are
// editing. It is declared as a pure virtual, so we can/must use it to make OUR kind of
// document.
void iconEdit::createDocObj(void) { 
	
	char* newFilePath;
	
	newFilePath = numberedFilePath(ICON_FLDR,"icon",".bmp");											// Looking at our folder, create an unused file name.
	if (newFilePath) {																							// If we got that name..
		if (createNewBMPFile(newFilePath,32,32)) {														// Setup that file as a blank, readable .BMP file.
			ourDoc = new iconEditScr(EDITSCR_X,EDITSCR_Y,EDITSCR_W,EDITSCR_H,newFilePath);	// Create our actual docFileObj.
			if (ourDoc) ourDoc->setAsAutoGen();																// This is an auto generated file. It'll be auto destructed later.
		}																												//
		resizeBuff(0,&newFilePath);																			// Recycle our copy of the file path.
	}
}


void iconEdit::createNewDocFile(void) {

	char* newFilePath;
 	bool	success;
 	
 	success = false;
	newFilePath = NULL;
	ourDoc->closeDocFile();													// We close what we have.. (If anything)
	newFilePath = numberedFilePath(ICON_FLDR,"icon",".bmp");		// Looking at our folder, create an unused file name.
	if (newFilePath) {														// If we got that name..
		if (createNewBMPFile(newFilePath,32,32)) {					// Set up the file as a readable .bmp image.
			if (ourDoc->changeDocFile(newFilePath)) {					// If we can change to this file..
				success = ourDoc->openDocFile(FILE_WRITE);			// Success is if we can open this file for editing.
				ourDoc->setAsAutoGen();										// This is an auto generated file. It'll be auto destructed later.
			}																		//
		}																			//
		resizeBuff(0,&newFilePath);										// Recycle the path buff.
	}
	if (success) {
		comID = okCmd;
	} else {
		comID = cancelCmd;
	}
	haveComToPassOn = true;
}


// setup() & loop() panel style.
void iconEdit::setup(void) {
ST
	documentPanel::setup();
	if (ourDoc) {
		addObj((iconEditScr*)ourDoc);
	}
}


void iconEdit::loop(void) {
	
	if (haveComToPassOn) {
		documentPanel::handleCom(comID);
		((iconEditScr*)ourDoc)->setNeedRefresh();
		haveComToPassOn = false;
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


