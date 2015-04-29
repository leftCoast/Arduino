#include "sprit.h"


sprit::sprit(void) {  }

sprit::~sprit(void) {  }

void sprit::startSprit(void) {  }

boolean sprit::active(void) { return false; }



// ********* sprit8x8 ********* 
  
  
sprit8x8::sprit8x8(Adafruit_8x8matrix* inMatrix) { 

  matrix = inMatrix;
  starting = false;
}
                        
                        
sprit8x8::~sprit8x8(void) {  }
    
    
void sprit8x8::startSprit(void) {

  starting = true;
  frameNum = 0;
  hookup();
}


boolean sprit8x8::active(void) { 

  boolean result;
  
  result = false;
  if (frameNum<numFrames) {             // Not fnished with frames, active..
    result = true;
  } else if (frameNum=numFrames) {     // Waiting for the last frame to expire.   
    result = !frameTimer.ding();
  } else {                             // Past last frame? Lets kill this.
    result = false;
  }
  return result;
}


void sprit8x8::idle(void) { }


boolean sprit8x8::changeFrame(void) {
   
  if (starting) {
    starting = false;
    return(true);
  } else if (frameNum<numFrames) {
    return frameTimer.ding();
  }   
} 


void sprit8x8::showFrame(const uint8_t* bitmap,unsigned long Ms) {
 
  matrix->clear();
  matrix->drawBitmap(0, 0, bitmap, 8, 8, LED_ON);
  matrix->writeDisplay();
  frameTimer.setTime(Ms);
  frameTimer.start();
  frameNum++;
}

