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

class bmpObj {
    
public:
    bmpObj(char* inFileName,point inDest);
    ~bmpObj(void);
    
    void setDest(point inDest);
    void getInfo(void);
    void plotLine(int y);
    void plotBmp(void);
    //void outputInfo(void);
    
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
