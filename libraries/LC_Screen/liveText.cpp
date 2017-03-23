#include "liveText.h"

liveText::liveText(int x,int y,word width,word height,int framerateMs,bool inLoop)  
  : label(x,y,width,height),
  idler(),
  colorMultiMap(),
  timeObj(framerateMs) {

  loop        = inLoop;
  maxTime     = 0;
  frame       = 0;
  holding     = false;
}


liveText::~liveText(void) {  }


// Need some running call to auto hookup.
boolean   liveText::wantRefresh(void) {
  
  if (!hookedIn) {
    hookup();
  }
  return label::wantRefresh();
}


void  liveText::addAColor(int timeMs,colorObj* color) { 

    if (timeMs>maxTime) {
      maxTime = timeMs;
    }
    colorMultiMap::addColor(timeMs,color);
  }


void liveText::hold(void) { holding = true; }

 
void liveText::release(bool reset) {

  holding = false;    // Take off the breaks.. 
  if (reset) {
    frame = 0;
  }
  setCalcColor();     // Just in case we get a draw call.
}

void liveText::drawSelf(void) {

  if(!holding) {
    label::drawSelf();
  }
}

void  liveText::setCalcColor(void) {
  
  colorObj  aColor;

  aColor = Map(frame);       // Calculate the color.
  setColors(&aColor);        // set it.
}


void liveText::idle(void) {
  
  
  if (ding()&&!holding) {                 // First check the timer.
    start();                              // Reset the timer from now. Allows hold to work.
    if (frame<=maxTime) {                 // Are we past the frame limit? If so, bail.
      setCalcColor();                     // And set the color;
      frame = frame + (getTime()/1000.0); // Step the frame counter. Units are ms.
      if (frame>maxTime) {                // We've come to the end. 
        if (loop) {                       // If we loop, we start over.
          frame = 0;                      // If so reset the frame thing.
        } else {                          // If we on't loop.
          hold();                         // We hold.
        }
      }
    }
  }
}

