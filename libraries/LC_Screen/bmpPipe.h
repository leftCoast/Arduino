#ifndef bmpPipe_h
#define bmpPipe_h

// We want to use bitmaps from SC cards for all sorts of drawing.
// So the idea behind this class is take care of everything about
// getting bitmaps to the screen. Where to get them, how big they
// are, where they should go. Should they be stored in RAM? Etc.
//
// For now we expect the correct or only SC card reader to be
// running. We also won't do any scaling of bitmaps. Just grab 
// rect A on the card and spit it to rect B on the display.

#include <SD.h>
#include <drawObj.h>
#include "screen.h"


class bmpPipe {

  public:
    bmpPipe(void);
    bmpPipe(rect sourceRect);
    ~bmpPipe(void);

    		boolean   		openPipe(char* fileName);
    		void      		setSourceRect(rect inRect);
virtual	void      		drawBitmap(int x,int y);
				File					getFile(void);
				void      		drawLine(File bmpFile,int x,int y);
    		unsigned long	filePtr(int x,int y);
      	//void      	showPipe(void);
    
  protected:
    uint16_t  		read16(File f);
    uint32_t  		read32(File f);
    boolean   		readInfo(File bmpFile);
    
    
    char*		filePath;
    rect    sourceRect;     // The subset of the image.
    boolean haveSourceRect;
    boolean haveInfo;
    int     imageOffset;
    int     imageWidth;
    int     imageHeight;
    int     imageDepth;
    int     pixBytes;
    int     bytesPerRow;
};

#endif

