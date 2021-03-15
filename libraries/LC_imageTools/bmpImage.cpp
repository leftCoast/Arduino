#include "bmpImage.h"

// .bmp files have the 2 & 4 byte numbers stored in reverse byte order
// than what we use here in Arduinoland. These two routines swap 'em back.

// For 2 byte numbers.
uint16_t read16(File f) {

    uint16_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
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
  
  

bmpImage::bmpImage(void)
: baseImage { }


bmpImage::~bmpImage(void) {  }


RGBpack bmpImage::getPixel(int x,int y,File* inFile) {

	RGBPack	aPack;
	File		ourFile;
	
	aPack.r = 0;										// We need something to pass back. Just in case.
	aPack.g = 0;
	aPack.b = 0;
	if (haveInfo) {									// If we were able to read the image file (earlier)..
		if (inFile) {									// If we were given an already open file to read..
			inFile.seek(fileIndex(x,y));			// Point the file at the pixel.
			inFile.read(buf,pixBytes);				// Grab the pixel.
			aPack.r = buf[2];							// Stuff the pack.
			aPack.g = buf[1];
			aPack.b = buf[0];
		} else {											// Else, no open file to read from. We'll need to open it.
			ourFile = SD.open(filePath);			// Open up the file.
			if (ourFile) {								// If we opened it.
				ourFile.seek(fileIndex(x,y));		// Point the file to our pixel.
				ourFile.read(buf,pixBytes);		// Grab the pixel.
				aPack.r = buf[2];						// Stuff the pack.
				aPack.g = buf[1];
				aPack.b = buf[0];
				ourFile.close();
			}
		}
	}
	return aPack;
}





// Grab a pixel from this location from your image file.
// Return it as a RGBpack.
// THIS ONE MUST BE FILLED OUT BY WHOM INHERITS THIS.
RGBpack baseImage::getPixel(int x,int y,File* inFile) {  }
   

// Given an RGBPack, set this pixel in the temp image file to that color.
// THIS is the one that should be inherited and filled out.
bool baseImage::setPixel(int x,int y,RGBpack* anRGBPack,File* imageFile) { }



bool bmpImage::checkDoc(void) {

	File		bmpFile;
	uint32_t	temp; 
	bool		success;
	
	success = false;														// Ok, assume failure..									
	bmpFile = SD.open(imagePath);										// See if its a valid path.
	if (bmpFile) {    													// We got a file?
		if (read16(bmpFile) == 0x4D42) {								// If we have something or other..
			fileSize = read32(bmpFile);								// We grab the file size.
			creatorBits = read32(bmpFile);							// Creator bits
			imageOffset = read32(bmpFile);							// image offset (Why read it into temp first?)
			DIBHeaderSize = read32(bmpFile);							// read DIB header size?
			width = read32(bmpFile);									// width? Good thing to save for later.
			height = read32(bmpFile);									// Height? Negative means the data is right side up. Go figure..
			rightSideUp = height<0;										// Set a bool about that bit.
			height = abs(imageHeight);									// Bloody weird encrypting.
			if (read16(bmpFile) == 1) {								// if the next word is 1? What's this mean? Needs to be though.
				imageDepth = read16(bmpFile);							// color depth.
				if (imageDepth == 24 || imageDepth == 32) {		// We can do 24 or 32 bits..
					pixBytes = imageDepth / 8;							// Bytes / pixel
					if (!read32(bmpFile)) {								// And no compression!
						bytesPerRow = imageWidth * pixBytes;		// Data takes up this much, but..
						while(bytesPerRow % 4) bytesPerRow++;		// We need to pad it to a multiple of 4.
						success = true;									// Made it this far? We can do this!
					}
				}
			}
		}
	}
	return success;
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


