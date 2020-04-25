#include "bmpObj.h"
#include "resizeBuff.h"

bmpObj::bmpObj(int inX,int inY,int inWidth,int inHeight,char* bmpPath)
	: drawObj(inX,inY,inWidth,inHeight),
	bmpPipe() {
	
	rect	sourceRect;
	
	sourceRect.setRect(0,0,inWidth,inHeight);			// Default source is top left of bitmap, our size.
	setSourceRect(&sourceRect);							// Set that up.
	mPath = NULL;												// Init to NULL for resizeBuff().
	if (bmpPath) {												// IF we got a path..
		if (resizeBuff(strlen(bmpPath)+1,&mPath)) {	// If we can get the RAM..
			strcpy(mPath,bmpPath);							// We save a copy of the path for later.
		}
	}
}

		
bmpObj::~bmpObj(void) { resizeBuff(0,&mPath); }
	

// Some stuff must wait 'till our hardware is up and running. like SD cards.	
bool	bmpObj::begin(void) {

	if (haveInfo) return true;		// Someone already took care of this? Good!
	if (mPath) {						// I guess they didn't.. So, if we have a file path..
		if (openPipe(mPath)) {		// If we can open the pipe with our file path..
			resizeBuff(0,&mPath);	// Then we can dump the copy of our path. Its in the bmpPipe now.
			return true;				// And we return success.
		}
	}
	return false;						// If we land here? Something went wrong.
}


void	bmpObj::drawSelf(void) { drawImage(x,y); }