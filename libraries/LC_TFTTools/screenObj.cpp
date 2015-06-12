#include "screenObj.h"

screenObj* screen;  // Now we have a global screen so we don't have
                    // to pass the silly thing around all the time.


// ****** screenObj ******

//Sets up the global screen object for us to use.
boolean initScreen(int inRotation) {

  screen = new screenObj();
  if (screen)
    return(screen->init(inRotation));
   else
   return(false);
}


screenObj::screenObj(void) :
    Adafruit_ILI9341(TFT_CS, TFT_DC) {
        
    ts = new Adafruit_FT6206();
}


boolean screenObj::init(int inRotation) {

    begin();
    if (!ts->begin(40)) {  // pass in 'sensitivity' coefficient
        return false;
    }
    setRotation(inRotation);
    return true;
}


void screenObj::fillRectGradient(word inX,word inY,word inXSize,word inYSize,colorObj* startColor,colorObj* endColor,boolean vertical) {

  colorMapper gradient(startColor, endColor);
  mapper posMapper(1,2,1,2);
  if (vertical) {
    posMapper.setValues(inY,inY+inYSize,0,100);
    for (int y=inY;y<inY+inYSize;y++) {
        drawFastHLine(inX, y, inXSize, (gradient.Map(posMapper.Map(y)).getColor16()));
    }
  } 
  else {
    posMapper.setValues(inX,inX+inXSize,0,100);
    for (int x=inX;x<inX+inXSize;x++) {
      drawFastVLine(x, inY, inYSize, (gradient.Map(posMapper.Map(x)).getColor16()));
    }
  }
}


void screenObj::fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical) {

  fillRectGradient(0,0,_width,_height,startColor,endColor,vertical);
}


/*
 void screenObj::drawPixelInvert(word x,word y) {

  word color;

  
  color = readPixel(x,y);
  color = ~color;
  drawPixel(x,y,color);
}

void screenObj::frameRectInvert(TS_Point loc, word width,word height) {

  word traceX;
  word traceY;
  word endX;
  word endY;

  
  endX = loc.x + width;
  endY = loc.y + height;
  traceX = loc.x;
  traceY = loc.y;
  while(traceX<endX) {
    drawPixelInvert(traceX,loc.y);
    drawPixelInvert(traceX,endY);
    traceX++;
  }
  while(traceY<endY) {
    drawPixelInvert(loc.x,traceY);
    drawPixelInvert(endX,traceY);
    traceY++;
  }
}
*/

TS_Point screenObj::getPoint(void) {

  TS_Point pt;

  pt = ts->getPoint();
  switch (getRotation()) {
  case INV_PORTRAIT:
    break;
  case INV_LANDSCAPE:
    swap(pt.x,pt.y);
    pt.x = pt.x;
    pt.y = height() - pt.y;
    break;
  case PORTRAIT:
    pt.x = width() - pt.x;
    pt.y = height() - pt.y;
    break;
  case LANDSCAPE:
    swap(pt.x,pt.y);
    pt.x = width() - pt.x;
    pt.y = pt.y;
    break;
  }
  return(pt);
}


boolean screenObj::touched(TS_Point inPt) {
    
    if (ts->touched()) {
        inPt = getPoint();
        return true;
    } else {
        return false;
    }
}


