#include <baseImage.h>
#include <resizeBuff.h>



// Just setup default values..
baseImage::baseImage(void) {

	width		= DEF_WIDTH;
	height	= DEF_HEIGHT;
}

		
// Dump the path strings.
baseImage::~baseImage(void) { }


// If the user wants to create a new file for the source colors. This is called
// to set the width and height for creating the temp image file that is used for
// editing.
bool baseImage::newImage(int inWidth,int inHeight) {
	
	width		= inWidth;				// Width known.
	height	= inHeight;				// Height known.
	return newDocFile();
}

		
// Grab a pixel from this location from your image file. Return it as a colorObj.
// Typically this is what the outside user would call.
colorObj baseImage::getPixel(int x,int y) {

	colorObj	aColor;
	RGBpack	aPack;
	File		imageFile;
	
	aColor.setColor(&black);								// Black's a good default.
	if(checkXYLmits(x,y)) {									// Make sure the x,y params are "sane".
		if (haveInfo) {										// If we have a valid image file.
			imageFile = SD.open(docFilePath,FILE_READ);	//	Try to open this file for reading.
			if (imageFile) {									// If we were able to open the image file..
				aPack = getPixel(x,y,&imageFile);		// Ask inherited to give us the packed pixel.
				imageFile.close();							// Close the image file.
				aColor.setColor(&aPack);					// Set it up as a colorObj.
			}
		}
	}
	return aColor;												// return the colorObj.
}


// Grab a row of pixels from this image (Or the temp) file.
bool baseImage::getRow(int row,RGBpack* RGBArray,int numPix,int xStart,bool fromTemp) {

	File	imageFile;
	int	xMax;
	int	arrayIndex;
	bool	success;
	bool	opendFile;
	
	success = false;
	if (haveInfo) {																// If we have a valid image file..
		if(checkRowParams(row,numPix,xStart)) {							// Make sure the row params are "sane".
			opendFile = false;													// Not opened a file as yet.
			if (fromTemp) {														// If we're supposed ot get this from the temp file..
				if (tempFilePath) {												// If we even have a temp file..
					imageFile = SD.open(tempFilePath,FILE_READ);			//	Try to open this file for reading.
					if (imageFile) {												// If we were able to open the temp file..
						opendFile = true;											// Note that we opened it.
					}
				}
			} else {
				imageFile = SD.open(docFilePath,FILE_READ);				//	Try to open this file for reading.
				if (imageFile) {													// If we were able to open the doc file..
					opendFile = true;												// Note that we opened it.
				}
			}
			if (opendFile) {														// If we were able to open the image file..
				xMax = xStart + numPix;											// Calculate the endpoint.
				arrayIndex = 0;													// Start array index at zero.
				for(int i=xStart;i<xMax;i++) {								// Loop through all the pixels..
					RGBArray[arrayIndex] = getPixel(i,row,&imageFile);	// Pass in the row, offset and file, get back the pixel.
					arrayIndex++;													// Bump up the array index.
				}
				imageFile.close();												// Clean up our mess.
				success = true;													// At this point we're calling it a success.
			}
		}
	}
	return success;
}


// Given a colorObj, set this pixel in the temp image file to that color.
bool baseImage::setPixel(int x,int y,colorObj* aColor) {

	RGBpack	aPack;
	File		imageFile;
	bool		success;
	
	success = false;														// Not a success yet.
	if (checkXYLmits(x,y)) {											// Limit x & y to "sane" values.
		if (haveInfo) {													// If we have a valid image file..
			if (createTempFile()) {										// If we have or can create a temp file..	
				imageFile = SD.open(tempFilePath,FILE_WRITE);	// Have a go at opening the temp file.
				if (imageFile) {											// If we got the temp file..
					aPack = aColor->packColor();						// Pack up the color.
					setPixel(x,y,&aPack,&imageFile);					// Set the pixel with this color into this file.
					imageFile.close();									// Close the file.
					success = true;										// Looks good!
				}
			}
		}
	}
	return success;													// Pass back our result.	
}


// Given an RGBpack array, set this row of pixels in the temp image file to
// these colors.
bool baseImage::setRow(int row,RGBpack* RGBArray,int numPix,int xStart) {
		
	File	imageFile;
	int	xMax;
	int	arrayIndex;
	bool	success;
	
	success = false;
	if (haveInfo) {																								// If we have a valid image file..
		if(checkRowParams(row,numPix,xStart)) {															// Make sure the row params are "sane".
			imageFile = SD.open(tempFilePath,FILE_WRITE);												//	Try to open this file for reading.
			if (imageFile) {																						// If we were able to open the image file..
				xMax = xStart + numPix;																			// Calculate the endpoint.
				arrayIndex = 0;																					// Start array index at zero.
				for(int i=xStart;i<xMax;i++) {																// Loop through all the pixels..
					setPixel(i,row,&(RGBArray[arrayIndex]),&imageFile);								// Set the pixel with this color into this file.
					arrayIndex++;																					// Bump up the array index.
				}
				imageFile.close();																				// Clean up our mess.
				success = true;																					// Success is assured!
			}
		}
	}
	return success;
}


// Grab a pixel from this location from your image file.
// Return it as a RGBpack.
// THIS ONE MUST BE FILLED OUT BY WHOM THAT INHERITS THIS.
RGBpack baseImage::getPixel(int x,int y,File* inFile) {  }
   

// Given an RGBpack, set this pixel in the temp image file to that color.
// THIS is the one that should be inherited and filled out.
void baseImage::setPixel(int x,int y,RGBpack* anRGBPack,File* imageFile) { }


bool baseImage::checkXYLmits(int x, int y) {

	if (x < 0 || x >= width) return false;		// If x is negative or past the end, bail.
	if (y < 0 || y >= height) return false;	// If y is negative or past the bottom, bail.
	return true;										// If we got this far, call it good.
}


// Is this necessary? Maybe an out of bounds should just give back black instead of
// stopping the code.
bool baseImage::checkRowParams(int row,int numPix,int xStart) {
	
	if (row<0 || row>=height) return false;			// If row is negative or greater than the height, bail.
	if (numPix<1 || numPix>width) return false;		// If numPix is zero or negative or greater than the width, bail.
	if (xStart<0 || xStart>=width) return false;		// If xStart is negative or greater than or then same as width, bail.
	if (width-xStart!=numPix) return false;			// If width minus xStart does not equal the numPix. bail.
	return true;												// If we get this far we'll call it good!
}

