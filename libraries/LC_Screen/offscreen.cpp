#include "offscreen.h"
#include "screen.h"


// ***************************************************************
// mapDisplay : Mimics a display to catch the drawing.
// ***************************************************************

// Constructor is only used by the offscreen class and it'll do the
// sanity checking before calling.
mapDisplay::mapDisplay(bitmap* inMap)  
  : Adafruit_GFX(inMap->getWidth(),inMap->getHeight()) {  mBitmap = inMap; }


mapDisplay::~mapDisplay(void) {  }


bool mapDisplay::dispObjBegin(void) { return true; }


void mapDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {

  colorObj  aColor(color);

  mBitmap->setColor(x,y,&aColor);
}


void mapDisplay::setFColor(colorObj* inColor) { mFColor.setColor(inColor); }


void mapDisplay::setBColor(colorObj* inColor) { mBColor.setColor(inColor); }


void mapDisplay::setTransp(bool transp) { mTransp = transp; }          



// ***************************************************************
// offscreen : The part the user interacts with.
// ***************************************************************

    
offscreen::offscreen(void)
  : displayObj(true,true,false,false,true) {
    
  oldScreen = NULL;
  mDisplay  = NULL;
  mDrawing  = false;
}


offscreen::~offscreen(void) { 
  
  if (mDisplay) delete mDisplay;
}


// Accept a bitmap, sanity check eveerything. If everything
// is good, set up for drawing to it.
void offscreen::beginDraw(bitmap* inMap) {

  if (mDrawing) {                         // We already drawing? Someone forgot to shut down?
    endDraw();                            // Shut it down.
  }
  if (mDisplay) {                         // If we have a display object..
    delete mDisplay;                      // dump it.
    mDisplay = NULL;                      // Note it's gone.
  }
  if (inMap) {                            // Sanity. We got a bitmap?
    if (inMap->getHasMap()) {             // And its RAM's been allocated?
      mDisplay  = new mapDisplay(inMap);  // Create the display objecet with the bitmap.
      if (mDisplay) {                     // Sanity. We got that display?
        oldScreen = screen;               // Everything seems ok. Save off the screen pointer.
        screen = this;                    // Replace it with us.
        mDrawing = true;                  // Note that we are now rerouting drawing calls.
      }
    }
  }
}


// ENOUGH! We're done drawing to the bitmap.
void offscreen::endDraw(void) {
  
  if (mDrawing) {         // Have we rerouted drawing calls?
    screen = oldScreen;   // Undo the screen rerouting.
    oldScreen = NULL;     // Note this is done.
    mDrawing = false;     // Note that we are no longer rerouting drawing calls.
  } 
}


// In case someone asks.
bool offscreen::dispObjBegin(void) { return true; }


// The standard pass throughs.
void offscreen::setRotation(byte inRotation)                                                  { mDisplay->setRotation(inRotation); }
void offscreen::setTextColor(colorObj* inColor)                                               { mDisplay->setTextColor(inColor->getColor16()); }
void offscreen::setTextColor(colorObj* tColor,colorObj* bColor)                               { mDisplay->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void offscreen::setTextSize(byte inSize)                                                      { mDisplay->setTextSize(inSize); }
void offscreen::setTextWrap(boolean wrap)                                                     { mDisplay->setTextWrap(wrap); }
void offscreen::setFont(const GFXfont* font)                                                  { mDisplay->setFont(font); }
void offscreen::setCursor(int x,int y)                                                        { mDisplay->setCursor(gX(x),gY(y)); }
void offscreen::drawText(char* inText)                                                        { mDisplay->print(inText); }
void offscreen::fillScreen(colorObj* inColor)                                                 { mDisplay->fillScreen(inColor->getColor16()); }
void offscreen::fillRect(int x,int y,int width,int height,colorObj* inColor)                  { mDisplay->fillRect(gX(x),gY(y), width, height,inColor->getColor16()); }
void offscreen::fillRect(rect* aRect,colorObj* inColor)                                       { mDisplay->fillRect(gX(aRect->x),gY(aRect->y), aRect->width, aRect->height,inColor->getColor16()); }
void offscreen::drawRect(int x,int y,int width,int height,colorObj* inColor)                  { mDisplay->drawRect(gX(x),gY(y), width, height,inColor->getColor16()); }
void offscreen::drawRect(rect* aRect,colorObj* inColor)                                       { mDisplay->drawRect(gX(aRect->x),gY(aRect->y), aRect->width, aRect->height,inColor->getColor16()); }
void offscreen::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)  { mDisplay->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void offscreen::fillRoundRect(rect* aRect,int radius,colorObj* inColor)                       { mDisplay->fillRoundRect(gX(aRect->x),gY(aRect->y),aRect->width,aRect->height,radius,inColor->getColor16()); }
void offscreen::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)  { mDisplay->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void offscreen::drawRoundRect(rect* aRect,int radius,colorObj* inColor)                       { mDisplay->drawRoundRect(gX(aRect->x),gY(aRect->y),aRect->width,aRect->height,radius,inColor->getColor16()); }
void offscreen::drawCircle(int x,int y,int diam, colorObj* inColor)                           { drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void offscreen::fillCircle(int x,int y,int diam, colorObj* inColor)                           { fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void offscreen::drawVLine(int x,int y,int height,colorObj* inColor)                           { mDisplay->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void offscreen::drawHLine(int x,int y,int width,colorObj* inColor)                            { mDisplay->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void offscreen::drawLine(int x,int y,int x2,int y2,colorObj* inColor)                         { mDisplay->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
void offscreen::drawPixel(int x,int y,colorObj* inColor)                                      { mDisplay->drawPixel(gX(x),gY(y),inColor->getColor16()); }


 
