#ifndef bmpObj_h
#define bmpObj_h

#include <SD.h>
#include "screenObj.h"

// The chip select pin for the SD card on the shield
#define SD_CS 5 

extern boolean initSDCard(void);

class SPIControl {
  
  public:
    SPIControl(void);  // After calling initSDCard() this will enable the SPI
    ~SPIControl(void);  // This will disable the SPI
};


class bmpObj {
  
  public:
    bmpObj(char* inFileName, TSPoint inDest);
    ~bmpObj(void);
    
    void setDest(TSPoint inDest);
    void getInfo(void);
    void plotLine(int y);
    void plotBmp(void);
    
    char*   fileName;
    File    source;
    TSPoint   dest;
    boolean haveInfo;
    int    imageOffset;
    int    imageWidth;
    int    imageHeight;
    int    imageDepth; 
    int     pixBytes;
};

#endif


