#include <baseImage.h>

// Just setup default values..
baseImage::baseImage(const char* filePath)
	: docFileObj(filePath) {

	width		= 0;
	height	= 0;
}

		
// Nothing to do at this level..
baseImage::~baseImage(void) { }


int baseImage::getWidth(void) { return width; }


int baseImage::getHeight(void) { return height; }

		
// Grab a pixel from this location from your image file. Return it as a colorObj.
// Typically this is what the outside user would call.
colorObj baseImage::getPixel(int x,int y) {

	colorObj	aColor;
	RGBpack	aPack;

	aColor.setColor(&black);				// Black's a good default.
	if (mode != fClosed) {					// If the file's open for anything..	
		if(checkXYLmits(x,y)) {				// Make sure the x,y params are "sane".
			aPack = getRawPixel(x,y);			// Ask inherited to give us the packed pixel.
			aColor.setColor(&aPack);		// Set it up as a colorObj.
		}
	}
	return aColor;								// return the colorObj.
}


// Given a colorObj, set this pixel to that color. (In the editing file)
bool baseImage::setPixel(int x,int y,colorObj* aColor) {

	RGBpack	aPack;
	bool		success;
	
	success = false;										// Not a success yet.
	if (mode==fOpenToEdit || mode==fEdited) {		// If the file's open for editing..
		if (checkXYLmits(x,y)) {						// If the point is within the image..											
			aPack = aColor->packColor();				// Pack up the color.
			setRawPixel(x,y,&aPack);					// Set the pixel.
			success = true;								// Looks good!
		}
	}
	return success;										// Pass back our result.
}


// Grab a row of pixels from this image.
bool baseImage::getRow(int x,int y,int numPix,RGBpack* RGBArray) {

	int	xMax;
	int	arrayIndex;
	bool	success;
	
	success = false;													// Not a success yet.
	if (mode != fClosed) {											// If the file's open for anything..
		xMax = x + numPix;											// Calculate xMax.
		if (checkXYLmits(x,y)) {									// If the start point is within the image..
			if (checkXYLmits(xMax-1,y)) {							// If the end point is ALSO within the image..
				arrayIndex = 0;										// Start array index at zero.
				for(int i=x;i<xMax;i++) {							// Loop through all the pixels..
					RGBArray[arrayIndex] = getRawPixel(i,y);	// Pass in the row, offset and file, get back the pixel.
					arrayIndex++;										// Bump up the array index.
				}
				success = true;										// At this point we're calling it a success.
			}
		}
	}
	return success;													// Pass back our result.
}


// Given an RGBpack array, set this row of pixels in the temp image file to
// these colors.
bool baseImage::setRow(int x,int y,int numPix,RGBpack* RGBArray) {
		
	int	xMax;
	int	arrayIndex;
	bool	success;
	
	success = false;													// Not a success yet.
	if (mode==fOpenToEdit || mode==fEdited) {					// If the file's open for editing..
		xMax = x + numPix;											// Calculate xMax.
		if (checkXYLmits(x,y)) {									// If the start point is within the image..
			if (checkXYLmits(xMax,y)) {							// If the end point is ALSO within the image..
				arrayIndex = 0;										// Start array index at zero.
				for(int i=x;i<xMax;i++) {							// Loop through all the pixels..
					setRawPixel(i,y,&(RGBArray[arrayIndex]));	// Pass in the row, offset and file, get back the pixel.
					arrayIndex++;										// Bump up the array index.
				}
				success = true;										// At this point we're calling it a success.
			}
		}
	}
	return success;													// Pass back our result.
}


bool baseImage::checkXYLmits(int x, int y) {

	if (x < 0 || x >= width) return false;		// If x is negative or past the end, bail.
	if (y < 0 || y >= height) return false;	// If y is negative or past the bottom, bail.
	return true;										// If we got this far, call it good.
}