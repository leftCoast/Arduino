#include <iconEdit.h>
#include <resizeBuff.h>

//#include <debug.h>

#define COLOR_BTN_X		EDITSCR_X
#define COLOR_BTN_Y		EDITSCR_Y + EDITSCR_H + 10
#define COLOR_BTN_NAME	"color32.bmp"

#define BRUSH_SLDR_X		COLOR_BTN_X + 50
#define BRUSH_SLDR_Y		COLOR_BTN_Y + 7
#define BRUSH_SLDR_W		140
#define BRUSH_SLDR_H		18


// Our .bmp file filter.
bool iconEditFilter(pathItem* inItem) {
	
	if (appleFilter(inItem)) {
		if (inItem->getType()==fileType) {
			return hasExtension(inItem->getName(),".BMP");
		} else {
			return true;
		}
	}
	return false;
}



// **************************************************************
// **********************    colorBtnObj    *********************
// **************************************************************


colorBtnObj::colorBtnObj(iconEdit* inEditor)
	: iconButton(COLOR_BTN_X,COLOR_BTN_Y,inEditor->getColorPickerPath()) {
	
	ourEditor = inEditor;
	setMask(&(ourOSPtr->icon32Mask));
}


colorBtnObj::~colorBtnObj(void) {   }
	
	
void colorBtnObj::doAction(void) {  ourEditor->openColorPicker(); }



// **************************************************************
// ************************    iconEdit    **********************
// **************************************************************

	
// And it all starts up again..
iconEdit::iconEdit(lilOS* ourOS,int ourAppID)
	: documentPanel(ourAppID) {
	
	strBuff = NULL;
	haveComToPassOn = false;
	setDefaultPath(ICON_FLDR);
	setFilter(iconEditFilter);
	ourState = editing;
	percentToBrush.setValues(0,100,1,6);
	brushToPercent.setValues(1,6,0,100);
}



// The world as you know it, is ending..
iconEdit::~iconEdit(void) { resizeBuff(0,&strBuff); }


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
	ourState = newDoc;
	haveComToPassOn = true;
}


// setup() & loop() panel style.
void iconEdit::setup(void) {
	
	documentPanel::setup();
	if (ourDoc) {
		addObj((iconEditScr*)ourDoc);
		colorBtn = new colorBtnObj(this);
		addObj(colorBtn);
		brushSlider = new slider(BRUSH_SLDR_X,BRUSH_SLDR_Y,BRUSH_SLDR_W,BRUSH_SLDR_H);
		brushSlider->setValue(brushToPercent.map(1));
		addObj(brushSlider);
	}
}


void iconEdit::loop(void) {
	
	int brush;
	
	if (haveComToPassOn) {
		handleCom(comID);
		((iconEditScr*)ourDoc)->setNeedRefresh();
		haveComToPassOn = false;
	}
	if (strBuff) {												// If we find a strBuff..
		resizeBuff(0,&strBuff);								// Deallocate it.
	}																// Its only used for passing back string info.
	brush = round(percentToBrush.map(brushSlider->getValue()));
	if (brush != ((iconEditScr*)ourDoc)->brushSize) {
		((iconEditScr*)ourDoc)->setBrushSize(brush);	
	}										
}


// The default here is to not draw ourselves. You can change that.
void iconEdit::drawSelf(void) {

	colorObj	aColor;
	rect		aRect;
	
	aRect.setRect(this);						// Lets grab our rectangle.
	aRect.y = aRect.y + 22;					// For menu..
	aRect.height = aRect.height - 22;	// For menu..
	aColor.setColor(LC_GREY);				// Make it a nice color.
	screen->fillRect(&aRect,&aColor);	// Draw it!
}


// This is used to find the color picker button graphic we use on our screen.
char* iconEdit::getColorPickerPath(void) {

	char*	ourFolder;
	int	numChars;
	
	ourFolder = ourOSPtr->getPanelFolder(getPanelID());	// Path to our folder on this system.
	numChars = strlen(ourFolder);									// Count the chars.
	numChars = numChars + strlen(COLOR_BTN_NAME);			// Add the chars of the button name.
	numChars++;															// And one for '\0'.
	if (resizeBuff(numChars,&strBuff)) {						// If we can allocate the RAM..
		strcpy(strBuff,ourFolder);									// Copy in the folder path.
		strcat(strBuff,COLOR_BTN_NAME);							// Add the name.
	}																		// Take a deep breath..
	return strBuff;													// And pass back the string buffer.
}


void iconEdit::openColorPicker(void) {
	
	colorObj	aColor;
	
	if (ourState == editing) { 
		aColor.setColor(&(((iconEditScr*)ourDoc)->editColor));
		colorAlert = new colorPicker(this);
		colorAlert->setColor(&aColor);
		ourState = getColor;
	}
}


void iconEdit::editingMode(stdComs comID) {
	documentPanel::handleCom(comID);
}


void iconEdit::newDocOpen(stdComs comID) {

	documentPanel::handleCom(comID);
	ourState = editing;
}


void iconEdit::colorOpen(stdComs comID) {
	
	if (comID==okCmd) {
		((iconEditScr*)ourDoc)->editColor = colorAlert->result();
	}
	ourState = editing;
}


// Handle the commands from the buttons and dialog boxes..
void iconEdit::handleCom(stdComs comID) {

	switch(ourState) {
		case editing	: editingMode(comID);	break;
		case newDoc		: newDocOpen(comID);		break;
		case getColor	: colorOpen(comID);		break;
	}
}



