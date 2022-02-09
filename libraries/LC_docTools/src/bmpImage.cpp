#include "bmpImage.h"
#include <resizeBuff.h>
#include <strTools.h>

//#include <debug.h>


#define DEF_IMAGE_OFFSET	54
#define DEF_DEPTH				32
#define COLOR_BUF_SIZE		4   // When grabbing a color off a bitmap you get 3 or 4 bytes. Go big!

// .bmp files have the 2 & 4 byte numbers stored in reverse byte order than what we use
// here in Arduinoland. These routines swap 'em back. (Sure seems like they aren't doing
// anything to me though..
// 
// For 2 byte numbers.
// uint16_t read16(File f) {
// 
//     uint16_t result;
//   
//     ((uint8_t *)&result)[0] = f.read(); // LSB
//     ((uint8_t *)&result)[1] = f.read(); // MSB
//     return result;
//   }
// 
// 
// void write16(uint16_t val, File f) {
//   
//    f.write(((uint8_t *)&val)[0]);
// 	f.write(((uint8_t *)&val)[1]);
// }
// 
// 
// For 4 byte numbers.
// uint32_t read32(File f) {
//   
//     uint32_t result;
//   
//     ((uint8_t *)&result)[0] = f.read(); // LSB
//     ((uint8_t *)&result)[1] = f.read();
//     ((uint8_t *)&result)[2] = f.read();
//     ((uint8_t *)&result)[3] = f.read(); // MSB
//     return result;
//   }
//   
//   
// void write32(uint32_t val, File f) {
//   
// 	f.write(((uint8_t *)&val)[0]);
// 	f.write(((uint8_t *)&val)[1]);
// 	f.write(((uint8_t *)&val)[2]);
// 	f.write(((uint8_t *)&val)[3]);
// }  



// **************************************************************************
// ********************** Create new .bmp file function *********************
// **************************************************************************

bool createNewBMPFile(const char* newPath,int inWidth,int inHeight) {

	File		bmpFile;
	bool		success;
	uint32_t	pixBytes;
	uint32_t	bytesPerRow;
	uint32_t	fileSize;
	uint32_t	imageBytes;
	byte buff[4];
	
	success = false;															// Ok, assume failure..									
	bmpFile = SD.open(newPath,FILE_WRITE);								// See if its a valid path.
	if (bmpFile) {    														// We got a file?
		bmpFile.seek(0);														// Write puts us at the end. Move to the beginning.
		
		// File header 14 bytes.
		write16(0x4D42,bmpFile);											// It wants this. Whatever.
		pixBytes		= DEF_DEPTH / 8;										// Calc. bytes per pixel.
		bytesPerRow	= inWidth * pixBytes;								// Calc bytes per row.
		while(bytesPerRow % 4) bytesPerRow++;							// We need to pad it to a multiple of 4.
		fileSize = DEF_IMAGE_OFFSET + (inHeight * bytesPerRow);	// Calc. fileSize.
		write32(fileSize,bmpFile);											// Calculate and write out the file size.
		write32(0,bmpFile);													// write out our creator bits. All I've seen are zero.
		write32(DEF_IMAGE_OFFSET,bmpFile);								// Image offset. Always seems to be 54. Size of header?
		
		// DIB Header
		write32(40,bmpFile);													// DIB Header size. Always seems to be 40. 40->BITMAPINFOHEADER
		write32(inWidth,bmpFile);											// Width.
		write32(inHeight,bmpFile);											// Height. (Positive, upside down)
		
		write16(1,bmpFile);													// It wants a one here. Why? Its a mystery to me.
		write16(DEF_DEPTH,bmpFile);										// We're going for 32 bit image stuff here.
		write32(0,bmpFile);													// No compression.
		imageBytes = bytesPerRow * inHeight;							// Calc. total image bytes.
		write32(imageBytes,bmpFile);										// Stuff it in.
		buff[3] = 0;															// Padding.
		while(bmpFile.position()<DEF_IMAGE_OFFSET) {					// Just in case we didn't make it to the right spot..
			bmpFile.write(&(buff[3]),1);									// Force the issue!
		}
		buff[0] = 255;	// BLUE												// We're going for the color white.
		buff[1] = 255; // GREEN												//
		buff[2] = 255; // RED												//																			// Now..
		for (int y=0;y<inHeight;y++) {									// We're going to..
			for (int x=0;x<inWidth;x++) {									// Go to every pixel
				bmpFile.write(buff,4);										// Stamp out the 4 byte color.
			}																		//
		}																			//
		bmpFile.close();														// Default file has been initialized. Close the file.
		success = true;														// Made it this far? We're calling this a success!
	}																				//
	return success;															// Report our results.
}



//****************************************************************************************
// bmpImage class:
//****************************************************************************************


bmpImage::bmpImage(const char* filePath)
	: baseImage(filePath) { newImgPath = NULL; }


bmpImage::~bmpImage(void) { freeStr(&newImgPath); }


// Setup for the next createNewDocFile() call.
void bmpImage::setPWH(const char* imgPath,int w,int h) { 

	heapStr(&newImgPath,imgPath);
	newW	= w;
	newH	= h;
}


bool bmpImage::setNewBMPFile(const char* BMPPath,int w,int h) {

	tempStr newPath(BMPPath);								// Save a local copy..
	
	closeDocFile();											// Close the original file, if any.
	if (createNewBMPFile(newPath.getStr(),w,h)) {	// If we can setup the new file..
		if (changeDocFile(newPath.getStr())) {			// If we can change to this new file..
			return  true;										// We'll call that a success.
		}															//
	}																//
	return false;												// Return our success.
}


// We get a general "We need a new file now" call. So using defaults..
bool bmpImage::createNewDocFile(void) { return setNewBMPFile(newImgPath,newW,newH); }


// Grab a pixel from this location from your image file.
// Return it as a RGBpack.
RGBpack bmpImage::getRawPixel(int x,int y) {
	
	uint8_t	buf[COLOR_BUF_SIZE];
	RGBpack	aPack;
	uint32_t	index;
	
	if (mode != fClosed) {			// If we are not closed. IE valid open file.
		index = fileIndex(x,y);		// Do the math for this pixel index.
		if (index != position()) {	// If we are NOT already there..
			seek(index);				// Point the file to our pixel.
		}									//
		read(buf,pixBytes);			// Grab the pixel.
		aPack.r = buf[2];				// Stuff the pack.
		aPack.g = buf[1];				//
		aPack.b = buf[0];				//
	}										//
	return aPack;						// Return the pack.
}


void bmpImage::setRawPixel(int x,int y,RGBpack* anRGBPack) {
	
	uint32_t	index;

	if (mode==fOpenToEdit || mode==fEdited) {		// If we are in an edit mode..
		index = fileIndex(x,y);							// Do the math for this pixel index.
		if (index != position()) {						// If we are NOT already there..
			seek(index);									// Point the file to our pixel.
		}														//
		write(anRGBPack->b);								// Write out the values.
		write(anRGBPack->g);								//
		write(anRGBPack->r);								//
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


