#include "flasher.h"


// *****************************************************
//                        flasher
// *****************************************************


flasher::flasher(rect* inRect,colorObj* backColor)
  : drawObj(inRect),
  blinker() {

  mBackColor.setColor(backColor);   // What is "off"?
  mForeColor.setColor(&red);        // Reasonable default.
}


flasher::flasher(int inX,int inY,int inWidth,int inHeight,colorObj* backColor)
  : drawObj(inX,inY,inWidth,inHeight),
  blinker() {
  
  mBackColor.setColor(backColor);   // What is "off"?
  mForeColor.setColor(&red);        // Reasonable default.
}

                     
flasher::~flasher(void) { }


// Basically a hacked version from blinker to remove the pinmode stuff.
// This is your on/off switch. Call with a boolean true=on false=off.
// The object is created in the "off" mode.
void flasher::setBlink(bool onOff) {
  
 if (!init) {             // Not intialized?
    hookup();             // Set up idling.
    init = true;          // Note it.
  }
  if((onOff!=running)) {  // ignore if no change
    if (onOff) {          // Start blinking..    
      start();            // Starting NOW!
      setLight(true);     // light on!
      onTimer->start();   // set the time on timer.
      running = true;     // Set state.
      } 
    else {                // Stop blinking..
      setLight(false);    // light off.
      running = false;    // set state.
    }
  }
}   

    
void flasher::setLight(bool onOff) {

  lightOn = onOff;
  setNeedRefresh();
}


void flasher::drawSelf(void) {

  if (lightOn) {
    screen->fillRect(x,y,width,height,&mForeColor);
  } else {
    screen->fillRect(x,y,width,height,&mBackColor);
  }
}



// *****************************************************
//                     bmpFlasher
// *****************************************************


bmpFlasher::bmpFlasher(int inX,int inY, int width,int height,char* onBmp, char* offBmp)
  : flasher(inX,inY,width,height,&black) { 
 
  mReady = false;
  setup(onBmp,offBmp);
}

    
bmpFlasher::bmpFlasher(rect* inRect,char* onBmp, char* offBmp)
  : flasher(inRect,&black) { 
  
  mReady = false;
  setup(onBmp,offBmp);
}  

    
bmpFlasher::~bmpFlasher(void) {

  if (mOnBmp) delete(mOnBmp);
  if (mOffBmp) delete(mOffBmp);
}


void bmpFlasher::setup(char* onBmp,char* offBmp) {

  rect  sourceRect(0,0,width,height);
  mOnBmp = new bmpPipe(&sourceRect);
  mOffBmp = new bmpPipe(&sourceRect);
  if (mOnBmp&&mOffBmp) {
    if (mOnBmp->openPipe(onBmp) && mOffBmp->openPipe(offBmp)) {
      mReady = true;
    }
  }
}


void bmpFlasher::drawSelf(void) {

  if (mReady) {
    if (lightOn) {
      mOnBmp->drawImage(x,y);
    } else {
      mOffBmp->drawImage(x,y);
    }
  }
}
