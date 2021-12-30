#include <iconEdit.h>
#include <resizeBuff.h>

#include <debug.h>

#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE


// For now we do this. Later we'll have more info.
bool iconEditFilter(const char* fileName) { return appleFilter(fileName); }



// And it all starts up again..
iconEdit::iconEdit(lilOS* ourOS,int ourAppID)
	: documentPanel(ourAppID) { ST }


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
		}																												//
		resizeBuff(0,&newFilePath);																				// Recycle our copy of the file path.
	}
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


void iconEdit::handleComHaveNamedFileNoEdits(stdComs comID) {
ST	
	char* newFilePath;
	
	newFilePath = NULL;
	switch(comID) {																						// Checking for each command..
		case newFileCmd	:																				// Ask for a new file to be created.
			newFilePath = numberedFilePath(ICON_FLDR,"icon",".bmp");							// Looking at our folder, create an unused file name.
			if (newFilePath) {																			// If we got that name..
				if (createNewBMPFile(newFilePath,32,32)) {										// Set up the file as a readable .bmp image.
					((iconEditScr*)ourDoc)->setPWH(newFilePath,32,32);							// Set up the document's new file defaults.
					if (ourDoc->createNewDocFile()) {												// If we can create this new file..
						ourState = haveFileNoNameNoEdits;											// Our state is now, have file, no name, no edits.
					}																							//
				}																								//
				resizeBuff(0,&newFilePath);															// Recycle the path buff.
			}																									//
		break;																								// Sign off.
		case openFileCmd	:																				// They want to open a different file..
			selectAlert = new selectFileObj(this,haveNamedFileNoEdits,comID,filter);	// Open the select file alert.
			ourState = selectOpen;																		// Our state is now selectAlert is open.
		break;																								// Exit.
		case saveFileCmd	: break;																		// Save nothing? Again, pointless. Exit.
		default: panel::handleCom(comID);															// Everything else we pass up the chain.
	}
}


void iconEdit::handleComAskOpen(stdComs comID) {
ST
	char* newFilePath;
	
	newFilePath = NULL;
	switch(comID) {																					// With an Alert open, we only check the okCmd & cancelCmd
		case okCmd			:																			// Ok ws cliked..
			ourDoc->closeDocFile();																	// Close the file, loose the changes.
			ourState = fileClosed;																	// Right now, our state is fileClosed.
			if (askAlert->getLastComID()==newFileCmd) {										// They were asking for a new file to be created..
				newFilePath = numberedFilePath(ICON_FLDR,"icon",".bmp");					// Looking at our folder, create an unused file name.
				if (newFilePath) {																	// If we got that name..
					if (createNewBMPFile(newFilePath,32,32)) {								// Set up the file as a readable .bmp image.
						((iconEditScr*)ourDoc)->setPWH(newFilePath,32,32);					// Set up the document's new file defaults.
						if (ourDoc->createNewDocFile()) {										// If we can create this new file..
							ourState = haveFileNoNameNoEdits;									// Our state is now, have file, no name, no edits.
						}																					//
					}																						//
					resizeBuff(0,&newFilePath);													// Recycle the path buff.
				}																							//																				//
			} else if (askAlert->getLastComID()==openFileCmd) {							// They wanted to open a different file..
				selectAlert = new selectFileObj(this,fileClosed,openFileCmd,filter);	// Open the select file alert.
				ourState = selectOpen;																// Now our state is select open.
			} else if (askAlert->getLastComID()==closeCmd) {								// They wanted the panel closed..
				panel::handleCom(closeCmd);														// This one, we pass up the chain.
			}																								//
		break;																							// Done, Whew!
		case cancelCmd		:																			// Cancel was clicked..
			ourState = askAlert->getLastState();												// Return to previous state.
		break;																							//
		default: panel::handleCom(comID);														// Everything else we pass up the chain.
	}
}



void  iconEdit::handleCom(stdComs comID) { ST documentPanel::handleCom(comID); }


