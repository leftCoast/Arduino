#include <baseImage.h>
#include <resizeBuff.h>

char	tempDir[TEMP_FOLDER];
char	pathBuff[TEMP_PATH_CHARS];


// ***************************************************
// NEED TO:
//the generation of the temp path and the tempFile name stuff needs to be wrapped into the class.
//The rest of the code needs to be rewritten to use the open * close methods at the end of this file.
//Desturctor needs to close all files.
//bmp Stuff needs to deal with no longer passing files around and use the open/close mnethods.
//
//See you after FLORIDA.
//
//My past you.



// If we can find or create the temp files directory. Pass back true. Else we pass back
// false for a fail.
bool createTempDir(void) {

	File	theDir;
	bool	success;
	int	numChars;
	
	success = false;										// Not a success yet..

	// WAIT!! Patching bug here. If you pass name/ and it fails, it crashes! No '/' no crash.
	numChars = strlen(tempDir);                  // Lets see how long the path is.
	if (theDir[numChars-1]=='/') {					// If we have a traileing '/'..            
		theDir[numChars-1]='\0';						// Clip off the trailing, and yes correct, '/'.
	}
	//      
	if (theDir=SD.open(tempDir,FILE_READ)) {		// If we can open the path they gave us..
		if (theDir.isDirectory()) {               // If its a directory..
			numChars = strlen(tempDir);            // Lets see how long the path is now.
			if (tempDir[numChars-1]!='/') {        // If the last char is NOT a '/'..
				strcat(tempDir,"/");                // Add one..
			}
			success = true;
		}
		theDir.close();
	} else {
		success = SD.mkdir(tempDir);					// Try making the directory.
		numChars = strlen(tempDir);            	// Lets see how long the path is.
		if (theDir[numChars-1]!='/') {				// If we DON'T have a traileing '/'.. 
			strcat(tempDir,"/");                	// Add one..
		}
	}
	return success;
}
			
	
// Run this. If it can create a new file with an previously unused file name, it'll pass
// back true, leaving the full path to the new file in the pathBuff string. If not, it
// passes back false.
bool generateTempPath(void) {
	
	File		tempFile;
	timeObj	timeOut(FILE_SEARCH_MS);
	int		fileNum;
	bool		done;
	bool		success;
	
	success = false;													// As always, not a success yet.
	if (createTempDir()) {											//	If we can find or create the temp directory..
		done = false;													// Not done yet.
		fileNum = 0;													// File names are a number string with ".tmp" at the end.
		do {																// We loop around doing..
			strcpy(pathBuff,tempDir);								//	Start building the file path starting with the temp directory.
			strcat(pathBuff,itoa(++fileNum);						// Increment the file num the add it to the path.
			strcat(pathBuff,".tmp");								// Add ".tmp" to the path.
			tempFile = SD.open(pathBuff,FILE_READ);			//	Try to open this file for reading.
			if (tempFile) {											// If the file opened..
				tempFile.close(tempFile);							// We just close it and move on.
			} else {														// Else, we have a possible candidate here.
				done = true;											// Either its the real deal or a n error. In any case, we are done. 
				tempFile = SD.open(pathBuff,FILE_WRITE);		// Try to create the file we couldn't open.
				if (tempFile) {										// If we were able to create the file..
					tempFile.close(tempFile);						// Close it.
					success = true;									// And We'll call this a success!
				}
			}
		} while(!done);												// Loop while we are not done.
	}
	return success;													// return if we had success or not.
}


// Just setup default values..
baseImage::baseImage(void) {

	imagePath		= NULL;
	tempPath			= NULL;
	imageFileOpen	= false;
	tempFileOpen	= false;
	haveInfo			= false;
	width				= 0;
	height			= 0;
}

		
// Dump the path strings.
baseImage::~baseImage(void) {

	resizeBuff(0,&imagePath);		// Recycle the image path's RAM.
	if (tempPath) {					// If we had a working file..
		SD.remove(tempPath)			// Delete it from the SD card.
		resizeBuff(0,&tempPath);	// Recycle it's path's RAM.
	}
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
				haveInfo = readImage();						// See if we can open and understand the image file.
			}
		}
	}
	return haveInfo;											// Tell the user.
}


// Called by the user to save her changes. If no path is specified. Save to the original
// image file. If a path is specified, then save the resulting image to that file. The
// default (this) is to blindly paste the data from the tempFile to the resulting savefile
// path.
bool baseImage::saveImage(char* inPath=NULL) {

	File	saveFile;
	File	tempFile;
	bool	success;
	
	success  = false;												// Not a success yet.
	if (tempPath) {												// If we have a path to a temp image file.
		tempFile = SD.open(tempPath);							// Try opening up the temp image file.
		if (tempFile) {											// If we were successful at opening the temp image file.
			if (inPath) {											// If we were given a non NULL save file path..
				saveFile = SD.open(inPath,FILE_WRITE);		// Try opening up a new file at this new path.
			} else {													// Else we were NOT given a new image file path to use..
				saveFile = SD.open(imagePath,FILE_WRITE);	// Try opening up the original image file.		
			}
			if (saveFile) {										// If we were successful at opening up a save file..
				while(tempFile.avaliable()) {					// While not at the end of the temp image file..
					saveFile.write(tempFile.read());			// Read a byte from the temp file and write it to the save file.
				}
				saveFile.close();									// We opened the save file, we close it!
				success = true;									// And we're calling this a success!
			}
			tempFile.close();										// We opened the temp file, we close it!
		}
		return success;											// Tell the world our resulting success or failure.
	}
}

		
// Grab a pixel from this location from your image file.
// Return it as a colorObj.
colorObj bmpImage::getPixel(int x,int y,File inFile) {

	colorObj	aColor;
	RGBPack	aPack;
	
	aPack = getPixel(x,y,inFile);
	aColor.setColor(&aPack);
	return aColor;
}


// Grab a pixel from this location from your image file.
// Return it as a RGBpack.
RGBpack baseImage::getPixel(int x,int y,File inFile) {  }


// Grab a row of pixels from this image file. Optionally starting
// at xMin and ending at xMax. If xMax is zero, then the entire row.
void bmpImage::getRow(int row,RGBpack* RGBArray,int xMin=0,xMax=0,File inFile) {

	File	ourFile;
	int	arrayIndex;
	
	if (haveInfo) {
		if (row<0) row = 0;
		if (row>=height) row = height - 1;
		if (xMin<0) xMin = 0;
		if (xMax==0 || xMax>=width) xMax = width-1;
		if (xMax<xMin) return;
		arrayIndex = 0;
		if (inFile) {
			for(int i=xMin;i<xMax;i++) {
				RGBArray[arrayIndex] = getPixal(row,i,inFile);
				arrayIndex++;
			}
		} else {
			ourFile = SD.open(filePath);			// Open up the file.
			if (ourFile) {
				for(int i=xMin;i<xMax;i++) {
					RGBArray[arrayIndex] = getPixal(row,i,ourFile);
					arrayIndex++;
				}
				ourFile.close();						// We open it, we close it.
			}
		}
	}
}
	


// If any of the setting methods are called, you will need to call
// the createWorkingFile() to setup a working file for your edits.

// Given a colorObj, set this pixel in the temp image file to that color.
void baseImage::setPixel(int x,int y,colorObj* aColor) {  }


// Given an RGBPack, set this pixel in the temp image file to that color.
void baseImage::setPixel(int x,int y,RGBpack* anRGBPack) {  }


// Given an RGBPack array, set this row of pixels in the temp image file to
// these colors. Passing zeros into xMin & xMax defailts them to the entire row.
void baseImage::setRow(int row,RGBpack* RGBPackArray,int xMin,int xMax) {  }


// Used to create a temporary working file from an existing image file. The default (this)
// is to blindly copy the existing image file to the file located at tempPath.
bool baseImage::copyToWorkingFile(void) {

	bool	success;
	
	success = false;												// We're not a success yet.
	if (haveInfo) {												// If we have checked and ok'ed our image file..
		if (generateTempPath()) {								// If we are able to generate a temp file path..
			tempFile = SD.open(tempPath,FILE_WRITE);		// Have a shot at opening up our temp file.
			if (tempFile) {										// If we were successful at getting a temp file..
				ourFile = SD.open(filePath);					// Have a shot at opening up our image file.
				if (ourFile) {										// If we were successful at opening our image file..
					while(ourFile.avalibale()) {				// While w're not at the EOF of the image file..
						tempFile.write(ourFile.read());		// Write this byte from the image file to the temp file.
					}
					success = true;								// Ok, we'll call this a success.
					ourFile.close();								// We opened it, we close it.
				}
				tempFile.close();									// We opened it, we close it.
			}
		}
	}
	return success;												// Return our success or failure.
}


// Used to create a temporary working file from a height and width of pixels.	We can't
// know how this is done at this time. Those who inherit us should take care of it.			
bool newWorkingFile(int width,int height) {  }			


// Write a pixel to the temp file. Sadly, at this point, we have no idea how. Those who
// inherit us should be able to handle this.
void baseImage::writePixel(int x,int y,RGBpack* anRGBPack) {  }


// **********************************************
// These next four are the REQUIRED management of the two files we manage. The imageFile
// and the tempFile.	In this way the we can be sure that the files will be opened at the
// start of writing and reading processes. Otherwise, they may be opened and closed for
// every read and write.

	
bool baseImage::openImage(int mode) {

	if (!imageFileOpen) {
		imageFile = SD.open(imagePath,mode);
		if (imageFile) {
			imageFileOpen = true;
		}
	}
}


void baseImage::closeImage(void) {

	if (imageFile) {
		imageFile.close();
	}
	imageFileOpen = false;;
}


bool baseImage::openTemp(int mode) {

	if (!tempFileOpen) {
			if (!tempPath) {
				
			tempFile = SD.open(tempPath,mode);
			if (tempFile) {
				tempFileOpen = true;
			}
		}
	}


void baseImage::closeTemp(void) {
					
	if (tempFile) {
		tempFile.close();
	}
	tempFileOpen = false;;
}			
	
// **********************************************