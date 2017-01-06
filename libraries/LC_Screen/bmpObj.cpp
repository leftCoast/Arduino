//
//  bmpObj.cpp
//  
//
//  Created by Jim Lee on 12/29/16.

#include "bmpObj.h"
#include <colorObj.h>

// Needed to draw bitmaps from SD card to screen. For lack of a better idea
// I just wrapped up all the code in here. Its a bit of a patchwok now.
// Maybe later I can find a better home for this.
//
// 12/29/16 - Not actually created now. It was written about 3 years ago and for
// some reason got deleted. Today I found it on Github and brought it back. "Whew!"


int8_t savedSPI;            // When we init, we'll set this.

boolean initSDCard(void) {
    
    if (SD.begin(SD_CS)) {  // This also sets up the SPCR reg.
        savedSPI = SPCR;      // Good this is what it wants..
        SPCR = 0;             // We're not using it now, stomp it.
        return true;
    }
    else {
        return false;
    }
}


// Constructor fires up the comunication port.
// This is accomplished by returning it to the
// State that SD.begin() left it in.
SPIControl::SPIControl(void) {
    SPCR = savedSPI;
}


// Destructor saves the comunication port and then shuts it down.
SPIControl::~SPIControl(void) {
    
    savedSPI = SPCR;
    SPCR = 0;
}

// Wrapper functions with the SPIControl calls.
File openSD(char* fileName) {
    
    SPIControl ourSPI;
    return(SD.open(fileName));
}

// Seems this reads two bytes.
int fileReadw(File inFile) {
    
    SPIControl ourSPI;
    return inFile.read();
}

// Move file pointer?
boolean fileSeek(File inFile, uint32_t pos) {
    
    SPIControl ourSPI;
    return(inFile.seek(pos));
}


// Grab a raw bufferload.
int readBuff(File inFile,void* buf, uint16_t nbyte) {
    
    SPIControl ourSPI;
    return(inFile.read(buf,nbyte));
}


// .bmp files have the 2 & 4 byte numbers stored in reverse byte order
// than what we use here in Arduinoland. These two routines swap 'em back.
// For 2 byte numbers.
uint16_t read16(File f) {
    uint16_t result;
    SPIControl ourSPI;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

// For 4 byte numbers.
uint32_t read32(File f) {
    uint32_t result;
    SPIControl ourSPI;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}


// Constructor for bmpObj. Takes a filename & point to draw it.
bmpObj::bmpObj(char* inFileName, point inDest) {
    
    fileName = (char*)malloc(strlen(inFileName+1)); // Grab just enoug RAM to store the string.
    strcpy(fileName,inFileName);                    // Save the filename.
    dest = inDest;                                  // Save the location we want to draw this.
    haveInfo = false;                               // We ain't read the file header yet..
}


// Do what we can to save memory.
bmpObj::~bmpObj(void) {
    free(fileName);
}


// Move the point used for locating the drawing.
void bmpObj::setDest(point inDest) {
    dest = inDest;
}


// This reads the .bmp file header info and decides if we can draw it or not.
void bmpObj::getInfo(void) {
    
    uint32_t temp;
    
    source = openSD(fileName);                    // Time to see if we can open the file.
    if (source) {                                 // We get a file handle?
        if (read16(source) == 0x4D42) {             // If we have something or other..
            temp = read32(source);                    // We grab the file size.
            temp = read32(source);                    // Creator bits
            temp = read32(source);                    // image offset (Why read it into temp first?)
            imageOffset = temp;                       // image offset!?! Save this! (Actually, this shuts up the compiler.)
            temp = read32(source);                    // read DIB header size?
            imageWidth = read32(source);              // width? Good thing to save for later.
            imageHeight = read32(source);             // Height? Negative means the data is right side up. Go figure..
            if (read16(source) == 1) {                // if the next word is 1? What's this mean? Needs to be though.
                imageDepth = read16(source);            // color depth.
                if (imageDepth==24||imageDepth==32) {   // We can do 24 or 32 bits..
                    pixBytes = imageDepth/8;              // Bytes / pixel
                    if (!read32(source)) {                // And no compression!
                        haveInfo = true;                    // Made it this far? We can do this!
                    }
                }
            }
        }
    }
}


// Called by plotBmp(), this does one line.
void bmpObj::plotLine(int y) {
    
    colorObj  thePixal;
    uint8_t   buf[pixBytes];
    int       x;
    
    for (x=dest.x; x<dest.x+imageWidth; x++) {            // Ok, x does dest to max image width.
        readBuff(source,buf,pixBytes);                    // Grab a pixel.
        thePixal.setColor(buf[2],buf[1],buf[0]);          // Load colorObj.
        screen->drawPixel(x,y,&thePixal);                 // Spat it out to the screen.
    }
}


// Plots the bitmap file to the screen.
void bmpObj::plotBmp(void) {
    
    int   y;
    
    if (haveInfo) {                                     // We sucessfully got the info?
        if (fileSeek(source,imageOffset)) {               // Move to the bitmap data.
            if (imageHeight>0) {                            // The image is upside down?
                for (y=dest.y+imageHeight; y>=dest.y; y--) {  // Loop from the bottom of the image to the top.
                    plotLine(y);                                // Draw this line.
                }
            }
            else {                                          // Image must be stored right side up.
                long temp = -imageHeight;                     // We need the absolute value. But don't loose the info.
                for (y=dest.y; y<=dest.y+temp; y++) {         // Loop from the top down.
                    plotLine(y);                                // Draw this line.
                }       
            }
        }
    }
}