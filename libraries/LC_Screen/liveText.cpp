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
  nextStr	  = NULL;
  erase		  = false;
}


liveText::~liveText(void) { if (nextStr) free(nextStr); }

void liveText::setValue(int value) { label::setValue(value); }


void liveText::setValue(char* str) {
	
	if (frame<maxTime) {										// We still have frames to go?
		if (str) {												// Ok, we do have something to print.
			nextStr = (char*)malloc(strlen(str)+1);	// Ask for some RAM.
			if (nextStr) {										// If we got it.
				erase = true;									// Set erase as a flag.
				strcpy(nextStr,str);							// Save off the new string for later.
				return;											// And we drop out of the function.
			}
		}
	}
	label::setValue(str);									// Otherwise? Just do as you always would.
}


// Need some running call to auto hookup.
bool   liveText::wantRefresh(void) {
  
  if (!hookedIn) {
    hookup();
  }
  return label::wantRefresh();
}


void  liveText::addAColor(int timeMs,colorObj* color) { 

    if (timeMs>maxTime) {
      maxTime = timeMs;
      lastColor = *color;
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
	
	if (erase) {						// Ok! If we need to change mid animation.
		setColors(&lastColor);		// Set to the ending color.
		label::drawSelf();			// This "should" erase the last text.
		label::setValue(nextStr);	// Now we mash in the new message. (We saved it.)
		free(nextStr);					// Free the buffer..
		nextStr = NULL;				// Flag the buffer..
		frame = 0;						// Reset to frame 0.
		setCalcColor();				// Calculate the starting color..
		erase = false;					// Everything should be ready.
	}
	label::drawSelf();
}


void  liveText::setCalcColor(void) {
  
  colorObj  aColor;

  aColor = Map(frame);       // Calculate the color.
  setColors(&aColor);        // set it.
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
				setColors(&lastColor);				// Set the last color;
				hold();									// We hold.
			}
		}
	}
}

