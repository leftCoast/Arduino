

bmpImage::bmpImage(void)
: baseImage { }


bmpImage::~bmpImage(void) {  }
	

bool bmpImage::openImage(char* inPath=NULL);
bool bmpImage::saveImage(char* inPath=NULL);
	
colorObj bmpImage::getPixel(int x,int y);
RGBpack bmpImage::getPixel(int x,int y);
void bmpImage::getRow(int row,RGBpack* RGBPackArray,int xMin=0,xMax=0);
	
void bmpImage::setPixel(int x,int y,colorObj* aColor);
void bmpImage::setPixel(int x,int y,RGBpack* anRGBPack);
void bmpImage::setRow(int row,RGBpack* RGBPackArray,int xMin=0,xMax=0);

bool bmpImage::readImage(void) {

	File		bmpFile;
	uint32_t	temp; 
	bool		success;
	
	success = false;														// Ok, assume failure..									
	bmpFile = SD.open(imagePath);										// See if it works.
	if (bmpFile) {    													// We got a file?
		if (read16(bmpFile) == 0x4D42) {								// If we have something or other..
			temp = read32(bmpFile);										// We grab the file size.
			temp = read32(bmpFile);										// Creator bits
			temp = read32(bmpFile);										// image offset (Why read it into temp first?)
			imageOffset = temp;											// image offset!?! Save this! (Actually, this shuts up the compiler.)
			temp = read32(bmpFile);										// read DIB header size?
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


bool bmpImage::openWorkingFile(void) {

}



// .bmp files have the 2 & 4 byte numbers stored in reverse byte order
// than what we use here in Arduinoland. These two routines swap 'em back.

// For 2 byte numbers.
uint16_t bmpImage::read16(File f) {

    uint16_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
  }


// For 4 byte numbers.
uint32_t bmpImage::read32(File f) {
  
    uint32_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
  }