//
//  bmpObj.h
//  
//
//  Created by Jim Lee on 12/29/16.
//
//

#ifndef bmpObj_h
#define bmpObj_h


#include <SD.h>
#include "screen.h"

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
    bmpObj(char* inFileName,point inDest);
    ~bmpObj(void);
    
    void setDest(point inDest);
    void getInfo(void);
    void plotLine(int y);
    void plotBmp(void);
    
    char*   fileName;
    File    source;
    point   dest;
    boolean haveInfo;
    int    imageOffset;
    int    imageWidth;
    int    imageHeight;
    int    imageDepth;
    int     pixBytes;
};


#endif /* defined(bmpObj_h) */
