#include "chainPixels.h"


// *******************************************
//
// chainPixels: For each chained group of
// neoPixel devices you create a chainPixels
// object to manage it.
//
// *******************************************


chainPixels::chainPixels(byte inPin) {
  
  pin = inPin;
  pixelDriver  = NULL;  // A NULL pixelDriver is the flag saying- "Things have changed"; Need reset.
  numPixels = 0;
  dirtyBit = false;
}


chainPixels::~chainPixels(void) { if (pixelDriver) delete pixelDriver; }
  
  
// If there is a change in configuration,
// reset all the values and things.
void chainPixels::resetChain(void) {
  
  linkListObj* trace;
  
  numPixels = 0;
  trace = peek();
  while(trace) {
    ((pixelGroup*)trace)->setIndex(numPixels);
    numPixels = numPixels + ((pixelGroup*)trace)->getNumPixels();
    trace = trace->next;
  }
  if (pixelDriver) {        // Just in case, we don't want to leak this.
      delete pixelDriver;
  }
  pixelDriver = new neoPixel(numPixels,pin);
  pixelDriver->begin();
}


// Hook into push to show a change in config.
void chainPixels::push(linkListObj* item) {
  
   if (pixelDriver) {       // Configuration is changing! Dump the driver!
      delete pixelDriver;
      pixelDriver = NULL;
   }
   queue::push(item);       // Then do what a queue would do.
}


// Hook into pop to show a change in config.
linkListObj* chainPixels::pop(void) {
 	  	
   if (pixelDriver) {        // Configuration is changing! Dump the driver!
      delete pixelDriver;
      pixelDriver = NULL;
    }
   return queue::pop();            // Then do what a queue would do.
}


// Adds a group. Groups must be added in the
// order that they are wired in.
void chainPixels::addGroup(pixelGroup* inGroup) {
  
  if (inGroup) {
    inGroup->setChain(this);      // Luke, I AM your father!
    push((linkListObj*)inGroup);  // And in it goes!
  }
}


// During idle time we give all the groups
// a slice to do stuff.
void chainPixels::idle(void) {
  
  linkListObj* trace;
  
  if (!pixelDriver) {        // Ok, do we need a reset? NULL diver is the flag.
    resetChain();
  }
  trace = peek();
  while(trace) {            // Now everyone gets time to change thigns if they want.
    ((pixelGroup*)trace)->draw();
    trace = trace->next;
  }
  if (dirtyBit) {          // If someone did change something..
    pixelDriver->show();   // Lest show it!
    dirtyBit = false;      // Cleanup.
  }
}


// Groups call this to see what color a pixel is in their set.
colorObj chainPixels::getPixelColor(word index) { return pixelDriver->getPixelColor(index); }
  

// Groups call this to set a color to a pixel in their set.  
void chainPixels::setPixelColor(word index,colorObj* inColor) {
  
  pixelDriver->setPixelColor(index,inColor);
  dirtyBit = true;
}
               
               

// *******************************************
//
// pixelGroup: This is the bit the user will
// inherit and change setPixels() to manage
// her lights.
//
// *******************************************


pixelGroup::pixelGroup(word inNumPixels) {
  
  numPixels = inNumPixels;
  ourChain = NULL;
  index = 0;
}

  
pixelGroup::~pixelGroup(void) { }
     
            
void pixelGroup::setIndex(word inIndex) { index = inIndex; }


void pixelGroup::setChain(chainPixels* inChain) { ourChain = inChain; }


word pixelGroup::getNumPixels(void) { return numPixels; }


colorObj pixelGroup::getPixelColor(word pixelNum) { return ourChain->getPixelColor(index+pixelNum); }


void pixelGroup::setPixelColor(word pixelNum, colorObj* color) { ourChain->setPixelColor(index+pixelNum,color); }


void pixelGroup::setPixels(colorObj* color) {

   for(word i=0;i<numPixels;i++) {
      setPixelColor(i,color);
   }
}
 
 
colorObj pixelGroup::shiftPixels(bool toEnd) {

	colorObj  aColor;
  colorObj  lastColor;
  int       last;

  last = getNumPixels()-1;
  if (toEnd) {                              // Feed new colors onto pixel 0, pull 'em through to the end.
    lastColor = getPixelColor(last);
    for(int i=last;i>0;i--) {
      aColor = getPixelColor(i-1);
      setPixelColor(i,&aColor);
    }
  } else {                                  // Feed new colors onto last pixel, push 'em to the start..
    lastColor = getPixelColor(0);
    for(int i=0;i<last;i++) {
      aColor = getPixelColor(i+1);
      setPixelColor(i,&aColor);
    }
  }
  return lastColor;
}


void pixelGroup::roll(bool clockwise)		{

		colorObj aColor;

  	aColor = shiftPixels(!clockwise);
  	if (!clockwise) {
  		setPixelColor(0,&aColor);
  	} else {
  		setPixelColor(getNumPixels()-1,&aColor);
  	}
  }
  						
  						 
void pixelGroup::draw(void) {  } // This will be called repeatedly. Fill with your drawing code.
 
