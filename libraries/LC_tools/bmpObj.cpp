#include "bmpObj.h" 

int8_t savedSPI;            // When we init, we'll set this.

boolean initSDCard(void) {

  if (SD.begin(SD_CS)) {
    savedSPI = SPCR;      // Good this is what it wants..
    SPCR = 0;             // We're not using it now, stomp it.
    return true;
  } 
  else {
    return false;
  }
}


SPIControl::SPIControl(void) { 
  SPCR = savedSPI; 
}


SPIControl::~SPIControl(void) {

  savedSPI = SPCR;
  SPCR = 0;
}


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


// Seems that the file thing swaps bytes and words. These swap 'em back when you read from it.
// Swap bytes 
uint16_t read16(File f) {
  uint16_t result;
  SPIControl ourSPI;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  SPIControl ourSPI;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}


bmpObj::bmpObj(char* inFileName, Point inDest) {

  fileName = (char*)malloc(strlen(inFileName+1));
  strcpy(fileName,inFileName);
  dest = inDest;
  haveInfo = false;
  imageOffset = 0;
}


bmpObj::~bmpObj(void) { 
  free(fileName); 
}


void bmpObj::setDest(Point inDest) { 
  dest = inDest; 
}


void bmpObj::getInfo(void) {

  uint32_t temp;

  source = openSD(fileName);                  // Time to see if we can open the file.
  if (source) {                                // We get a file handle?
    if (read16(source) == 0x4D42) {            // If we have something or other.. 
      temp = read32(source);                   // We grab the fiele size.
      temp = read32(source);                   // Creator bits
      imageOffset = read32(source);            // image offset
      temp = read32(source);                   // read DIB header size?
      imageWidth = read32(source);
      imageHeight = read32(source);            // negative means the data is right side up. Go figure..
      if (read16(source) == 1) {               // if the next word is 1? What's this mean?
        imageDepth = read16(source);           // color depth.
        if (imageDepth==24||imageDepth==32) {
          pixBytes = imageDepth/8;
          if (!read32(source)) {                 // And no compression!
            haveInfo = true;
          }
        }
      }
    }
  }
}

  
void bmpObj::plotLine(int y) {

  uint16_t  color16;
  uint8_t   buf[pixBytes];
  int       x;
  
  for (x=dest.x; x<dest.x+imageWidth; x++) {
    readBuff(source,buf,pixBytes);
    color16 = screen->Color565(buf[2],buf[1],buf[0]);
    screen->drawPixel(x,y,color16);
  }
}


void bmpObj::plotBmp(void) {

  int   y;
  
  if (haveInfo) {
    if (fileSeek(source,imageOffset)) {
      if (imageHeight>0) {
        for (y=dest.y+imageHeight; y>=dest.y; y--) {
          plotLine(y);
        }
      } 
      else {
        long temp = -imageHeight;
        for (y=dest.y; y<=dest.y+temp; y++) {
          plotLine(y);
        }
      }
    }
  }
}









