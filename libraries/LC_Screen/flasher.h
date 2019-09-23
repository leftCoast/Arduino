#ifndef flasher_h
#define flasher_h

#include <drawObj.h>
#include <blinker.h>
#include <bmpPipe.h>



// A twisted version of blinker that has the pin stuff masked and repurposed to use
// the timing code to swap colors on a colord object on a screen.

// *****************************************************
//                        flasher
// *****************************************************


// Copied directly from Flora_II maybe one day it'll be a library thing.
class flasher : public drawObj,
                public blinker {
    public:
                  flasher(rect* inRect,colorObj* backColor=&black);
                  flasher(int inLocX,int inLocY,int inWidth,int inHeight,colorObj* backColor=&black);
                     
    virtual       ~flasher(void);
    virtual void  setBlink(bool onOff);
    virtual void  setLight(bool onOff);
    virtual void  drawSelf(void);
    
            colorObj  mForeColor;
            colorObj  mBackColor;
};



// Tweaking the flasher above to swap between two bitmaps instead of a colored object on a screen.

// *****************************************************
//                     bmpFlasher
// *****************************************************


class bmpFlasher :  public flasher {
    
    public:
                  bmpFlasher(int inX,int inY, int width,int height,char* onBmp, char* offBmp);
                  bmpFlasher(rect* inRect,char* onBmp, char* offBmp);
    virtual       ~bmpFlasher(void);

            void  setup(char* onBmp, char* offBmp);
    virtual void  drawSelf(void);

            bool      mReady;
            bmpPipe*  mOnBmp;
            bmpPipe*  mOffBmp;     
};

#endif
