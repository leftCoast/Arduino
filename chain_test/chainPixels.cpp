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
  pixelDriver  = NULL;
  numPixels = 0;
  dirtyBit = false;
}


chainPixels::~chainPixels(void) { if (pixelDriver) delete pixelDriver; }
  
  
// If there is a deletion, this can be called
// to reset everyone's indexes and things.
// But really, how often would someone want to
// change the wiring of the different bits of
// hardware on the fly?
void chainPixels::resetGroups(void) {
  
  linkListObj* trace;
  
  numPixels = 0;
  trace = top();
  while(trace) {
    ((pixelGroup*)trace)->setIndex(numPixels);
    numPixels = numPixels + ((pixelGroup*)trace)->getNumPixels();
    trace = trace->next;
  }
  if (pixelDriver) {
      delete pixelDriver;
  }
  pixelDriver = new neoPixel(numPixels,pin);
  trace = top();
  while(trace) {
    ((pixelGroup*)trace)->setPixelDriver(pixelDriver);
    trace = trace->next;
  }
}


// Adds a group. Groups must be added in the
// order that they are wired in.
void chainPixels::addGroup(pixelGroup* inGroup) {
  
  if (inGroup) {
    push((linkListObj*)inGroup);
    inGroup->setIndex(numPixels);
    numPixels = numPixels + inGroup->getNumPixels();
    if (pixelDriver) {
      delete pixelDriver;
    }
    pixelDriver = new neoPixel(numPixels,pin);
    inGroup->setPixelDriver(pixelDriver);
    inGroup->setChain(this);
  }
}


// Groups can call this to note that something
// as changed and LEDs need to be re-written.
void chainPixels::needRefresh(void) { dirtyBit = true; }


// During idle time we give all the groups
// some time to do stuff.
void chainPixels::idle(void) {
  
  linkListObj* trace;
  
  trace = top();
  while(trace) {
    ((pixelGroup*)trace)->setPixels();
    trace = trace->next;
  }
  if (dirtyBit) {
    pixelDriver->show();
    dirtyBit = false;
  }
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
  pixelDriver = NULL;
  index = 0;
}

  
pixelGroup::~pixelGroup(void) { }
     
            
void pixelGroup::setIndex(word inIndex) { index = inIndex; }


void pixelGroup::setPixelDriver(neoPixel* inDriver) { pixelDriver = inDriver; }


void pixelGroup::setChain(chainPixels* inChain) { ourChain = inChain; }


word pixelGroup::getNumPixels(void) { return numPixels; }


void pixelGroup::setPixels(void) {  } // This will be called repeatedly. Fill with your drawing code.
 
