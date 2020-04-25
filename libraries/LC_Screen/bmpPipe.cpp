#include "bmpPipe.h"
#include <resizeBuff.h>
#include <colorObj.h>

#define COLOR_BUF_SIZE  4   // When grabbing a color off a bitmap you get 3 or 4 bytes. Go big!

bmpPipe::bmpPipe(void) { 
  
  filePath			= NULL;
  haveInfo			= false;
  haveSourceRect	= false;
}


bmpPipe::bmpPipe(rect* sourceRect) { 

  filePath			= NULL;
  haveInfo			= false;
  haveSourceRect	= false;
  setSourceRect(sourceRect);
}


bmpPipe::~bmpPipe(void) { if (filePath) resizeBuff(0,&filePath); }	// If we have a path, loose it.
 
 
bool bmpPipe::openPipe(char* filename) {

	rect aRect;
	File bmpFile;

	haveInfo = false;														// Ok, assume failure..									
	if (filePath) resizeBuff(0,&filePath);							// If we have a path, loose it.
	bmpFile = SD.open(filename);										// See if it works.
	if (bmpFile) {    													// We got a file?
		if (readInfo(bmpFile)) {										// Then see if we can understand it	
    		if (resizeBuff(strlen(filename)+1,&filePath)) {		// If we can grab storage for name.
    			strcpy(filePath,filename);								// Save it off.
    			haveInfo = true;											// success!
    		}
		if (haveInfo && !haveSourceRect) {  						// If we can..
			aRect.x = 0;                      						// Default the source to the image.
			aRect.y = 0;
      	aRect.width = imageWidth;
      	aRect.height = imageHeight;
			setSourceRect(&aRect);
		}
	}
		bmpFile.close();												// Done, thanks!
	}
	return haveInfo;                     					// Tell the caller if it worked.
}


// .bmp files have the 2 & 4 byte numbers stored in reverse byte order
// than what we use here in Arduinoland. These two routines swap 'em back.

// For 2 byte numbers.
uint16_t bmpPipe::read16(File f) {

    uint16_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
  }


// For 4 byte numbers.
uint32_t bmpPipe::read32(File f) {
  
    uint32_t result;
  
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
  }


// We'll hand it to you. You BETTER close it when you're done!
File bmpPipe::getFile(void) { 

	File bmpFile;
	if (haveInfo) {
		bmpFile = SD.open(filePath);
	}
	return bmpFile;
}


// When we
void bmpPipe::setSourceRect(rect* inRect) { 
  
	sourceRect = *inRect;
	haveSourceRect = true;
}


bool bmpPipe::readInfo(File bmpFile) {

    bool   success = false;
    uint32_t  temp;
  
    if (bmpFile) {												// We get a file handle?
      if (read16(bmpFile) == 0x4D42) {						// If we have something or other..
        temp = read32(bmpFile);								// We grab the file size.
        temp = read32(bmpFile);								// Creator bits
        temp = read32(bmpFile);								// image offset (Why read it into temp first?)
        imageOffset = temp;									// image offset!?! Save this! (Actually, this shuts up the compiler.)
        temp = read32(bmpFile);								// read DIB header size?
        imageWidth = read32(bmpFile);						// width? Good thing to save for later.
        imageHeight = read32(bmpFile);						// Height? Negative means the data is right side up. Go figure..
        if (read16(bmpFile) == 1) {							// if the next word is 1? What's this mean? Needs to be though.
          imageDepth = read16(bmpFile);					// color depth.
          if (imageDepth == 24 || imageDepth == 32) {	// We can do 24 or 32 bits..
            pixBytes = imageDepth / 8;						// Bytes / pixel
            if (!read32(bmpFile)) {							// And no compression!
              bytesPerRow = imageWidth * pixBytes;		// Data takes up this much, but..
              while(bytesPerRow % 4) bytesPerRow++;	// We need to pad it to a multiple of 4.
              success = true;									// Made it this far? We can do this!
            }
          }
        }
      }
    }
    return success;
  }


// Your standard do it one at a time.
void bmpPipe::drawLine(File bmpFile,int x,int y) {

    colorObj  thePixal;
    uint8_t   buf[COLOR_BUF_SIZE];   
    int       trace;
    int       endTrace;

    endTrace = x+sourceRect.width;
    for (trace=x;trace<endTrace; trace++) {       // Ok, trace does x..x+width.
      bmpFile.read(buf,pixBytes);                 // Grab a pixel.
      thePixal.setColor(buf[2],buf[1],buf[0]);    // Load colorObj.
      screen->drawPixel(trace,y,&thePixal);       // Spat it out to the screen.
	}
}


unsigned long bmpPipe::filePtr(int x,int y) {

	long  index;
  
	index = imageOffset;												// Starting here..
	if (imageHeight>0) {												// The image is upside down?
		index = index + ((imageHeight-y-1)*bytesPerRow);	// reverse..
	} else {
		index = index + (y*bytesPerRow);							// not reverse.
	}
	index = index + (sourceRect.x * pixBytes);				// Add offset for x.
	return index;														// Tell the world.
}


// Draw the image to the current screen. Used to be called drawBitmap. But that
// was a bad choice for a name.
void bmpPipe::drawImage(int x,int y) {
  
	File			bmpFile;
	int			trace;
	int			endY;
	int			srcY;
	rect			defRect;

	if (haveInfo) {										// If we have valid bmp info..
		bmpFile = SD.open(filePath);					// Open up the file.
		if (bmpFile) {										// If we opened it.
			endY = y+sourceRect.height;				// Start calculating endpoints and things.
			srcY = sourceRect.y;
			for (trace=y; trace<endY;trace++) {		// Ready to pull data through to the screen.
				bmpFile.seek(filePtr(x,srcY++));		// Position the file pointer to the line of pixels we want.
				drawLine(bmpFile,x,trace);				// Standard old pixel by pixel draw. (Least it works.)
			}
			bmpFile.close();								// Drawing is done for now. Close the file.
		}      
	} else {													// Else we don't have info for this..
		if (haveSourceRect) {
			defRect.setRect(x,y,sourceRect.width,sourceRect.height);
		} else {
			defRect.setRect(x,y,32,32);
		}
		screen->fillRect(&defRect,&white);
		screen->drawRect(&defRect,&red);
		screen->drawLine(&defRect.getCorner(topLeftPt),&defRect.getCorner(bottomRightPt),&red);
		screen->drawLine(&defRect.getCorner(topRightPt),&defRect.getCorner(bottomLeftPt),&red);
	}
}


#ifdef DEBUGGING

void bmpPipe::showPipe(void) {

  Serial.print("Filepath         : ");Serial.println(filePath);
  Serial.print("Src rect x,y,w,h : ");Serial.print(sourceRect.x);Serial.print(", ");Serial.print(sourceRect.y);
  Serial.print(", ");Serial.print(sourceRect.width);Serial.print(", ");Serial.println(sourceRect.height);
  Serial.print("Have src rect    : ");Serial.println(haveSourceRect);
  Serial.print("Have info        : ");Serial.println(haveInfo);
  Serial.print("image offset     : ");Serial.println(imageOffset);
  Serial.print("image width      : ");Serial.println(imageWidth);
  Serial.print("Image height     : ");Serial.println(imageHeight);
  Serial.print("Image depth      : ");Serial.println(imageDepth);
  Serial.print("Pix Bytes        : ");Serial.println(pixBytes);
  Serial.print("Bytes per Row    : ");Serial.println(bytesPerRow);
}

#endif
