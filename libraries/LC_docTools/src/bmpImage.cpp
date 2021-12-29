#include "bmpImage.h"
#include <resizeBuff.h>


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
// ********************** Create new .bmp file function *********************
// **************************************************************************

 
bool createNewBmpFile(char* newPath,int inWidth,int inHeight) {

	File		bmpFile;
	bool		success;
	uint32_t	pixBytes;
	uint32_t	bytesPerRow;
	uint32_t	fileSize;
	uint32_t	imageBytes;
	
	success = false;															// Ok, assume failure..									
	bmpFile = SD.open(newPath,FILE_WRITE);								// See if its a valid path.
	if (bmpFile) {    														// We got a file?
		write16(0x4D42,bmpFile);											// It wants this. Whatever.
		pixBytes		= DEF_DEPTH / 8;										// Calc. bytes per pixel.
		bytesPerRow	= inWidth * pixBytes;								// Calc bytes per row.
		while(bytesPerRow % 4) bytesPerRow++;							// We need to pad it to a multiple of 4.
		fileSize = DEF_IMAGE_OFFSET + (inHeight * bytesPerRow);	// Calc. fileSize.
		write32(fileSize,bmpFile);											// Calculate and write out the file size.
		write32(0,bmpFile);													// write out our creator bits. All I've seen are zero.
		write32(DEF_IMAGE_OFFSET,bmpFile);								// Image offset. Always sees to be 54. Size of header?
		write32(40,bmpFile);													// DIB Header size. Always seems to be 40.
		write32(inWidth,bmpFile);											// Width.
		write32(inHeight,bmpFile);											// Height.
		write16(1,bmpFile);													// It wants a one here. Why? Its a mystery to me.
		write32(DEF_DEPTH,bmpFile);										// We're going for 32 bit image stuff here.
		write32(0,bmpFile);													// No compression.
		imageBytes = bytesPerRow * inHeight;							// Calc. total image bytes.
		for (uint32_t i=0;i<imageBytes;i++) {							// For every bloody byte of pixel data..
			bmpFile.write(255);												// Fill image space with white.
		}
		bmpFile.close();														// Default file has been initialized. Close the file.
		success = true;														// Made it this far? We're calling this a success!
	}
	return success;															// Report our results.
}



// **************************************************************************
// ***************************** bmpImage class *****************************
// **************************************************************************


bmpImage::bmpImage(char* filePath)
	: baseImage(filePath) {  }


bmpImage::~bmpImage(void) {  }


// A inheritable method that sets the x,y size of a new image file.
void bmpImage::setNewFileParams(void) {
	newW = 32;
	newH = 32;
}


// Once the image params are setup. We can call this to create the file.
bool bmpImage::newDocFile(char* folderPath) {

	bool success;
	
	success = false;												// Not a success yet.
	closeDocFile();												// Close the original file, if any.
	if (createTempFile(folderPath,".BMP")) {				// If we can create a suitable path..
		if (createNewBmpFile(pathBuff,newW,newH)) {		// If, using this path we can create a new file..
			if (changeDocFile(pathBuff)) {					// If we can change to this new path..
				success = true;									// That's a success!
			}															//
		}																//
		resizeBuff(0,&pathBuff);								// Loose the buffer.
	}																	//
	return success;												// Return our success.
}


// Grab a pixel from this location from your image file.
// Return it as a RGBpack.
RGBpack bmpImage::getRawPixel(int x,int y) {
	
	uint8_t	buf[COLOR_BUF_SIZE];
	RGBpack	aPack;
	uint32_t	index;
	
	if (mode != fClosed) {							// If we are not closed. IE valid open file.
		index = fileIndex(x,y);						// Do the math for this pixel index.
		if (index != ourFile.position()) {		// If we are NOT already there..
			ourFile.seek(index);						// Point the file to our pixel.
		}
		ourFile.read(buf,pixBytes);				// Grab the pixel.
		aPack.r = buf[2];								// Stuff the pack.
		aPack.g = buf[1];
		aPack.b = buf[0];
	}
	return aPack;
}


// Given an RGBpack, set this pixel in the temp image file to that color.
// THIS is the one that should be inherited and filled out.
void bmpImage::setRawPixel(int x,int y,RGBpack* anRGBPack) {
	
	uint32_t	index;
	
	if (mode==fOpenToEdit || mode==fEdited) {		// If we are in an edit mode..
		index = fileIndex(x,y);							// Do the math for this pixel index.
		if (index != ourFile.position()) {			// If we are NOT already there..
			ourFile.seek(index);							// Point the file to our pixel.
		}
		ourFile.write(anRGBPack->b);					// Write out the values.
		ourFile.write(anRGBPack->g);
		ourFile.write(anRGBPack->r);
	}
}


// Open the doc file and see if its what we expect. Then grab out the info. we need to
// "deal" with it.
bool bmpImage::checkDoc(File bmpFile) {

	uint32_t	creatorBits;	// A thing that seems to always be zero.
	uint32_t	DIBHeaderSize;	// And so is this thing.
	int32_t	imageHeight;	// Used as a temp. Can be negative.
	int32_t	fileSize;
	int16_t	imageDepth;
	bool		success;
	
	success = false;													// Ok, assume failure..									
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
	return success;
}


// Given an x,y location of a pixel. Hand back the file
// index of its location.
uint32_t bmpImage::fileIndex(int x,int y) {

	uint32_t  index;
	
	index = imageOffset;											// Starting here..
	if (rightSideUp) {											// The image is right side up?
		index = index + (y*bytesPerRow);						// Right side up
	} else {															// Else..
		index = index + ((height-y-1)*bytesPerRow);		// Upside down.
	}
	index = index + (x * pixBytes);							// Add offset for x.
	return index;													// Tell the world.
}


