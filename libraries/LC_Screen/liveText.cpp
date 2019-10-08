#include "liveText.h"

liveText::liveText(int x,int y,int width,int height,int framerateMs,bool inLoop)  
  : label(x,y,width,height),
  idler(),
  colorMultiMap(),
  timeObj(framerateMs) {

  loop        = inLoop;
  maxTime     = 0;
  frame       = 0;
  holding     = false;
}


liveText::~liveText(void) { }



// Need some running call to auto hookup.
bool   liveText::wantRefresh(void) {
  
  hookup();
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


void  liveText::setCalcColor(void) {
  
	bool		savedTransp;
	colorObj	aColor;
	
	aColor = Map(frame);		// Calculate the color.
	savedTransp = transp;	// Save off our transparent setting.
	setColors(&aColor);		// set the color it.
	transp = savedTransp;	// Restore the transp setting.
}


void liveText::idle(void) {
  
  
	if (ding()&&!holding) {							// First check the timer.
		start();											// Reset the timer from now. Allows hold to work.
		if (frame<maxTime) {							// We still have frames to go?
			setCalcColor();							// Set the color;
			frame = frame + (getTime()/1000.0);	// Step the frame counter. Units are ms.
		} else {											// We're done with our frames.
			if (loop) {									// If we loop, we start over.
				frame = 0;								// If so, reset the frame thing.
			} else {										// If we don't loop.
				setCalcColor();						// Set the last color;
				hold();									// We hold.
			}
		}
	}
}

