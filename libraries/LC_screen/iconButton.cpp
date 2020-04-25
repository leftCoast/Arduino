#include <iconButton.h>

// Its a SQUARE icon. Used to be fixed at 32x32 but no..
// Now we want different sizes, don't we?
iconButton::iconButton(int xLoc,int yLoc,char* path,int pix)
	: drawObj(xLoc,yLoc,pix,pix,touchLift) {
	
	rect	source(0,0,pix,pix);
	int	numChars;
	
	setSourceRect(&source);					// Take bits from here.
	numChars = strlen(path);				// Save off the path.
	mPath = (char*)malloc(numChars+1);
	if (mPath) {
		strcpy(mPath,path);
	}
}
	
	
iconButton::~iconButton(void) { if(mPath) free(mPath); }
	

// We are created with just a path to the bitmap file. 
// Is the file system online yet? Don't know. So, we call
// this later, in hopes that by then, whom ever
// is in charge has everything sorted.
void iconButton::begin(void) {

	if (mPath) {
		openPipe(mPath);				// Open up the bitmap file and save everyone.
		if(haveInfo) {					// Did all this work?
			//Serial.print("Got pipe : ");Serial.println(mPath);
			free(mPath);				// If so, we don't need our copy of the path.
			mPath = NULL;
			return;
		}
	}
	//Serial.print("No Pipe : [");Serial.print(mPath);Serial.println("]");
	//showPipe();
}


// Blast out the bitmap.
void iconButton::drawSelf(void) { drawImage(x,y); }


void iconButton::doAction(void) {  }



iconButton32::iconButton32(int xLoc, int yLoc,char* path)
	: iconButton(xLoc,yLoc,path,32) {  }


iconButton32::~iconButton32(void) {  }



iconButton22::iconButton22(int xLoc, int yLoc,char* path)
	: iconButton(xLoc,yLoc,path,22) {  }


iconButton22::~iconButton22(void) {  }
