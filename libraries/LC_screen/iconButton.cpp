#include <iconButton.h>


iconButton::iconButton(int xLoc,int yLoc,char* path)
	: drawObj(xLoc,yLoc,32,32,true) {
	
	rect	source(0,0,32,32);
	int	numChars;
	
	setSourceRect(source);					// Take bits from here.
	numChars = strlen(path);				// Save off the path.
	mPath = (char*)malloc(numChars+1);
	if (mPath) {
		strcpy(mPath,path);
	}
}
	
	
iconButton::~iconButton(void) { if(mPath) free(mPath); }
	

// We are created with just a path to the bitmap file. 
// Is the file system online yet? Don't know. So, we call
// this later, in hopes that by then whom ever
// is in charge has everything sorted by then.
void iconButton::begin(void) {

	if (mPath) {
		openPipe(mPath);				// Open up the bitmap file and save everyone.
		if(haveInfo) {					// Did all this work?
			free(mPath);				// If so, we don't need our copy of the path.
			mPath = NULL;
		}
	}
}


// Check if we "haveInfo". If so, then draw.
void iconButton::drawSelf(void) { if(haveInfo) drawBitmap(x,y); }



void iconButton::doAction(void) {  }
