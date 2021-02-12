#include <baseImage.h>
#include <resizeBuff.h>


// Just setup default values..
baseImage::baseImage(void) {

	imagePath	= NULL;
	tempPath		= NULL;
	haveInfo		= false;
	width			= 0;
	height		= 0;
}

		
// Dump the path strings.
baseImage::~baseImage(void) {

	resizeBuff(0,&imagePath);
	resizeBuff(0,&tempPath);
}


// If the user wants to create a new file for the source colors. This is called
// to set the width and height for creating the temp image file that is used for
// editing.
bool baseImage::newImage(int inWidth,int inHeight) {
	
	resizeBuff(0,&imagePath);		// Just in case we're changing files.
	width		= inWidth;				// Width known.
	height	= inHeight;				// Height known.
	haveInfo	= true;					// We should have enough for a file.
	return openWorkingFile();		// Return if we can setup a working file.
}


// Your standard open image function. This saves off the file path then calls read image 
// to see if the file opened makes any sense.
bool baseImage::openImage(char* imPath) {

	int numChars;
	
	resizeBuff(0,&imagePath);								// Just in case we're changing files.
	haveInfo = false;											// No longer setup.
	if (inPath) {												// If we have a non NULL path pointer..
		numChars = strlen(inPath);							// See how long the path string is.
		if (numChars) {										// If we have any chars at all..
			if (resizeBuff(numChars+1,&imagePath)) {	// If we get the RAM to store the path..
				strcpy(imagePath,inPath);					// Save off the path.
				setup = readImage();							// See if we can open and understand the image file.
				return haveInfo;								// Tell the user.
			}
		}
	}
}


// Called by the user to save her changes. If no path is specified. Save to the
// original image file. If a path is specified then save the resulting image to
// that file. If possible.
bool baseImage::saveImage(char* inPath=NULL) {  }


// Grab a pixel from this location from your image file.
// Return it as a colorObj.
colorObj baseImage::getPixel(int x,int y) {  }

// Grab a pixel from this location from your image file.
// Return it as a RGBpack.
RGBpack baseImage::getPixel(int x,int y) {  }

// Grab a row of pixels from this image file. Optionally starting
// at xMin and ending at xMax. If xMax is zero, then the entire row.
void baseImage::getRow(int row,RGBpack* RGBPackArray,int xMin,int xMax) {  }
	

// If any of the setting methods are called, you will need to call
// the createWorkingFile() to setup a working file for your edits.

// Given a colorObj, set this pixel in the temp image file to that color.
void baseImage::setPixel(int x,int y,colorObj* aColor) {  }

// Given an RGBPack, set this pixel in the temp image file to that color.
void baseImage::setPixel(int x,int y,RGBpack* anRGBPack) {  }

// Given an RGBPack array, set this row of pixels in the temp image file to
// these colors. Passing zeros into xMin & xMax defailts them to the entire row.
void baseImage::setRow(int row,RGBpack* RGBPackArray,int xMin,int xMax) {  }


// Used to setup a temporary working file for image edits.
bool baseImage::openWorkingFile(void) {  }
					
					