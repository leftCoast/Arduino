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
 
 
 rect::rect(TSPoint location, word inWidth,word inHeight) {
   
   setLocation(location);
   setSize(inWidth,inHeight);
 }
 
 
 rect::~rect(void) {
   
 }
 
 
void  rect::setLocation(word x, word y) {
  
  location.x = x;
  location.y = y;
}


void  rect::setLocation(TSPoint inLoc) {

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

bool rect::inRect(TSPoint inPt) {

  return(
  inPt.x >= minX() &&
  inPt.x <= maxX() &&
  inPt.y >= minY() &&
  inPt.y <= maxY()
  );
}


// ***********************************

drawObj::drawObj() {

  needRefresh = true;                // Well Duh! We never been drawn yet!
}


drawObj::drawObj(TSPoint inLoc, word inWidth,word inHeight) : rect(inLoc,inWidth,inHeight) {

  needRefresh = true;
}


drawObj::~drawObj() { }


boolean  drawObj::getRefresh(void) { return(needRefresh); }

void  drawObj::setRefresh(void) { needRefresh = true; }


void  drawObj::draw(void) { 

  drawSelf();
  needRefresh = false;
}


// override this one and draw yourself.
void  drawObj::drawSelf(void) {
  
  screen->fillRect(location.x, location.y, width, height, BLACK); // Default draw.
  screen->drawRect(location.x, location.y, width, height, WHITE);
}



