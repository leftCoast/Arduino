#include "drawObj.h"

#define DEF_LOC_X  16    // Just anything so the user can see something.
#define DEF_LOC_Y  16
#define DEF_SIZE_X 16
#define DEF_SIZE_Y 16

 rect::rect(void) {
   
   setLocation(DEF_LOC_X,DEF_LOC_Y);
   setSize(DEF_SIZE_X,DEF_SIZE_Y);
 }
 
 
 rect::rect(word x, word y, word inWidth,word inHeight) {
   
   setLocation(x,y);
   setSize(inWidth,inHeight);
 }
 
 
 rect::rect(TS_Point location, word inWidth,word inHeight) {
   
   setLocation(location);
   setSize(inWidth,inHeight);
 }
 
 
 rect::~rect(void) { }
 
 
void  rect::setLocation(word x, word y) {
  
  location.x = x;
  location.y = y;
}


void  rect::setLocation(TS_Point inLoc) {

  location.x = inLoc.x;
  location.y = inLoc.y;
}


void  rect::setSize(word inWidth,word inHeight) {

  width = inWidth;
  height = inHeight;
}


void rect::setRect(rect inRect) {
  
  setLocation(inRect.location);
  setSize(inRect.location.x,inRect.location.y);
}


word rect::maxX(void) { return(location.x + width); }
word rect::maxY(void)  { return(location.y + height); }
word rect::minX(void) { return(location.x); }
word rect::minY(void)  { return(location.y); }

bool rect::inRect(TS_Point inPt) {

  return(
  inPt.x >= minX() &&
  inPt.x <= maxX() &&
  inPt.y >= minY() &&
  inPt.y <= maxY()
  );
}


// ***********************************

drawObj::drawObj() {

  needRefresh = true;       // Well Duh! We never been drawn yet!
  wantsClicks = false;    // 'Cause this is actually the default.
}


drawObj::drawObj(TS_Point inLoc, word inWidth,word inHeight,boolean inClicks)
    : rect(inLoc,inWidth,inHeight) {

    needRefresh = true;
    wantsClicks = inClicks;
}


drawObj::drawObj(word locX, word locY, word inWidth,word inHeight,boolean inClicks)
    : rect(locX,locY,inWidth,inHeight) {
    
    needRefresh = true;
    wantsClicks = inClicks;
}


drawObj::~drawObj() { }


// Call this one to draw..
void  drawObj::draw(void) {

  drawSelf();
  needRefresh = false;
}


// override this one and draw yourself.
void  drawObj::drawSelf(void) {
  
  screen->fillRect(location.x, location.y, width, height, BLACK); // Default draw.
  screen->drawRect(location.x, location.y, width, height, WHITE);
}


// Override me for action!
void drawObj::doAction(TS_Point where) {  }



// ***************************************************
// viewMgr, This is they guy that runs the screenshow.
// Handles redrawing, clicks, etc.
// ***************************************************

viewMgr::viewMgr(void) {
    
    theList = NULL;
    theTouched = NULL;
}


viewMgr::~viewMgr(void) {
    
    if (theList) {
        while(theList->next) delete theList->next;
        while(theList->prev) delete theList->prev;
        delete theList;
        theList = NULL;
    }
    theTouched = NULL;
}


// We're going to add from the bottom up.
void viewMgr::addObj(drawObj* newObj) {
    
    if (theList) {                      // We got a list?
        theList->linkToStart(newObj);   // Put the new guy at the top of the list.
    } else {
        hookup();                       // On our first addition, hook up idling.
    }
    theList = newObj;                   // Either way, point to the top of the list.
}

// We have time to do stuff, not a lot!
void viewMgr::idle(void) {
    
}



