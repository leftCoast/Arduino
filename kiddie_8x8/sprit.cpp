#include "sprit.h"


sprit::sprit(void) { spritRunning = false; }

sprit::~sprit(void) {  }

void sprit::startSprit(void) {  }

boolean sprit::active(void) { return spritRunning; }



// ********* sprit8x8 ********* 
  
  
sprit8x8::sprit8x8(Adafruit_8x8matrix* inMatrix) { matrix = inMatrix; }
                        
                        
sprit8x8::~sprit8x8(void) {  }
    
    
void sprit8x8::startSprit(void) {

  frameNum = -1;
  hookup();
  spritRunning = true;
  frameTimer.setTime(0);
}


boolean sprit8x8::active(void) { 

  boolean result;
  
  result = false;
  if (spritRunning)
    if (frameNum<numFrames) {
      result = true;
    } else if (frameNum=numFrames) {     // Waiting for the last frame to expire.   
      result = !frameTimer.ding();
    }
  }
  return result;
}


void sprit8x8::idle(void) { 

    if (frameTimer.ding()) {
      if (frameNum>numFrames) {
        spritRunning = false;
      } else {
        frameNum++;
        showFrame();
      }
    }
}


void sprit8x8::showBitmap(const uint8_t* bitmap,unsigned long Ms) {
 
  matrix->clear();
  matrix->drawBitmap(0, 0, bitmap, 8, 8, LED_ON);
  matrix->writeDisplay();
  frameTimer.setTime(Ms);
  frameTimer.start();
}

