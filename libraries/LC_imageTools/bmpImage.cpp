#include "bmpImage.h"

#define DEF_IMAGE_OFFSET	54
#define DEF_DEPTH				32
#define COLOR_BUF_SIZE		4   // When grabbing a color off a bitmap you get 3 or 4 bytes. Go big!

// .bmp files have the 2 & 4 byte numbers stored in reverse byte order
// than what we use here in Arduinoland. These two routines swap 'em back.

// For 2 byte numbers.
uint16_t read16(File f) {

    uint16_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
  }


void write16(uint16_t val, File f) {
  
   f.write(((uint8_t *)&val)[0]);
	f.write(((uint8_t *)&val)[1]);
}


// For 4 byte numbers.
uint32_t read32(File f) {
  
    uint32_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
  }
  
  
void write32(uint32_t val, File f) {
  
	f.write(((uint8_t *)&val)[0]);
	f.write(((uint8_t *)&val)[1]);
	f.write(((uint8_t *)&val)[2]);
	f.write(((uint8_t *)&val)[3]);
}  



// **************************************************************************
// ***************************** bmpImage class *****************************
// **************************************************************************  
  

bmpImage::bmpImage(void)
: baseImage() { gFileOpen = false; }


bmpImage::~bmpImage(void) {  }


// Grab a pixel from this location from your image file.
// Return it as a RGBpack.
RGBpack bmpImage::getPixel(int x,int y,File* imageFile) {
	
	uint8_t	buf[COLOR_BUF_SIZE];
	RGBpack	aPack;

	if (haveInfo) {							// If we were able to read the image file (earlier)..
		imageFile->seek(fileIndex(x,y));	// Point the file to our pixel.
		imageFile->read(buf,pixBytes);	// Grab the pixel.
		aPack.r = buf[2];						// Stuff the pack.
		aPack.g = buf[1];
		aPack.b = buf[0];
	}
	return aPack;
}


// Grab a row of pixels from this image (Or the temp) file. The default version of this
// works, but its slow. This is where we put our bmp file tweaks in to speed up the
// process.
bool bmpImage::getRow(int row,RGBpack* RGBArray,int numPix,int xStart,bool fromTemp) {

	File		imageFile;
	File*		imageFilePtr;
	uint8_t	buf[COLOR_BUF_SIZE];
	bool		success;
	
	success = false;
	imageFilePtr = NULL;
	
	if (haveInfo) {																// If we have a valid image file..
		if(checkRowParams(row,numPix,xStart)) {							// Make sure the row params are "sane".
			if (gFileOpen) {														// If there is a global file..
				imageFilePtr = gFile;											// We'll trust it for us to read from.
			} else if (fromTemp) {												// If we're supposed ot get this from the temp file..
				if (tempFilePath) {												// If we even have a temp file..
					imageFile = SD.open(tempFilePath,FILE_READ);			//	Try to open this file for reading.
					if (imageFile) {												// If we were able to open the temp file..
						imageFilePtr = &imageFile;								// Point to it.
					}
				}
			} else {
				imageFile = SD.open(docFilePath,FILE_READ);				//	Try to open this file for reading.
				if (imageFile) {													// If we were able to open the doc file..
					imageFilePtr = &imageFile;									// Point to it.
				}
			}
			if (imageFilePtr) {													// If we were able to open the image file..
				imageFilePtr->seek(fileIndex(xStart,row));				// Point the file to our pixel.
				for(int i=0;i<numPix;i++) {									// Loop through all the pixels..
					imageFilePtr->read(buf,pixBytes);						// Grab a pixel.
					RGBArray[i].r = buf[2];										// Stuff the pack.
					RGBArray[i].g = buf[1];
					RGBArray[i].b = buf[0];
				}
				if (!gFileOpen) {													// If there was no global file open..
					imageFilePtr->close();										// Clean up our mess.
				}
				success = true;													// At this point we're calling it a success.
			}
		}
	}
	return success;
}


// External code can call to set up an external file for a method to use. Calling this
// with NULL flips the boolean to false.
void bmpImage::setGFile(File* inFile) {

	gFile = inFile;
	if (inFile) {
		gFileOpen = true;
	} else {
		gFileOpen = false;
	}
}


// Given an RGBpack, set this pixel in the temp image file to that color.
// THIS is the one that should be inherited and filled out.
void bmpImage::setPixel(int x,int y,RGBpack* anRGBPack,File* imageFile) {
	
	if (haveInfo) {								// If we were able to read the image file (earlier)..
		imageFile->seek(fileIndex(x,y));		// Point the file to our pixel.
		imageFile->write(anRGBPack->b);		// Write out the values.
		imageFile->write(anRGBPack->g);
		imageFile->write(anRGBPack->r);
	}
}


// Open the doc file and see if its what we expect. Then grab out the info. we need to
// "deal" with it.
bool bmpImage::checkDoc(void) {

	File		bmpFile;
	uint32_t	creatorBits;	// A thing that seems to always be zero.
	uint32_t	DIBHeaderSize;	// Ans so is this ting.
	int32_t	imageHeight;	// Used as a temp. Can be negative.
	bool		success;
	
	success = false;														// Ok, assume failure..									
	bmpFile = SD.open(docFilePath);									// See if its a valid path.
	if (bmpFile) {    													// We got a file?
		if (read16(bmpFile) == 0x4D42) {								// If we have something or other..
			fileSize = read32(bmpFile);								// We grab the file size.
			creatorBits = read32(bmpFile);							// Creator bits
			imageOffset = read32(bmpFile);							// image offset.
			DIBHeaderSize = read32(bmpFile);							// read DIB header size?
			width = read32(bmpFile);									// width? Good thing to save for later.
			imageHeight = read32(bmpFile);							// Height? Negative means the data is right side up. Go figure..
			rightSideUp = imageHeight<0;								// Set a bool about that bit.
			height = abs(imageHeight);									// Bloody weird encrypting.
			if (read16(bmpFile) == 1) {								// if the next word is 1? What's this mean? Needs to be though.
				imageDepth = read16(bmpFile);							// color depth.
				if (imageDepth == 24 || imageDepth == 32) {		// We can do 24 or 32 bits..
					pixBytes = imageDepth / 8;							// Bytes / pixel
					if (!read32(bmpFile)) {								// And no compression!
						bytesPerRow = width * pixBytes;				// Data takes up this much, but..
						while(bytesPerRow % 4) bytesPerRow++;		// We need to pad it to a multiple of 4.
						success = true;									// Made it this far? We can do this!
					}
				}
			}
		}
	}
	return success;
}

				
void bmpImage::calcDefaults(void) {
	
	rightSideUp	= false;							// To make things easier, its up side down. (Positive value)
	imageOffset	= DEF_IMAGE_OFFSET;
	imageDepth	= DEF_DEPTH;
	pixBytes		= DEF_DEPTH / 8;
	bytesPerRow	= width * pixBytes;
	while(bytesPerRow % 4) bytesPerRow++;		// We need to pad it to a multiple of 4.
	fileSize = imageOffset + (height * bytesPerRow);
}


bool bmpImage::initNewTempFile(void) {

	File		bmpFile;
	RGBpack	aColor;
	bool		success;
	
	success = false;										// Ok, assume failure..									
	bmpFile = SD.open(tempFilePath,FILE_WRITE);	// See if its a valid path.
	if (bmpFile) {    									// We got a file?
		calcDefaults();									// Set up all the paramaters to file defaults.
		write16(0x4D42,bmpFile);						// It wants this. Whatever.
		write32(fileSize,bmpFile);						// Calculate and write out the file size.
		write32(0,bmpFile);								// write out our creator bits. All I've seen are zero.
		write32(imageOffset,bmpFile);					// Image offset. Always sees to be 54. Size of header?
		write32(40,bmpFile);								// DIB Header size. Always seems to be 40.
		write32(width,bmpFile);							// Width.
		write32(height,bmpFile);						// Height.
		write16(1,bmpFile);								// It wants a one here. Why? Its a mystery to me.
		write32(imageDepth,bmpFile);					// We're going for 32 bit image stuff here.
		write32(0,bmpFile);								// No compression.
		aColor.r = 255;									// Setup a white color pack.
		aColor.g = 255;
		aColor.b = 255;
		for (int y = 0;y<height;y++) {				// Loop through every pixel..
			for (int x=0;x<width;x++) {
				setPixel(x,y,&aColor,&bmpFile);		// Set it white.
			}
		}		
		bmpFile.close();									// Default file has been initialized. Close the file.
		success = true;									// Made it this far? We're calling this a success!
	}
	return success;										// Report our results.
}


// Given an x,y location of a pixel. Hand back the file
// index of its location.
unsigned long bmpImage::fileIndex(int x,int y) {

	unsigned long  index;
  
	index = imageOffset;											// Starting here..
	if (rightSideUp) {											// The image is right side up?
		index = index + (y*bytesPerRow);						// Right side up
	} else {															// Else..
		index = index + ((height-y-1)*bytesPerRow);		// Upside down.
	}
	index = index + (x * pixBytes);							// Add offset for x.
	return index;													// Tell the world.
}


