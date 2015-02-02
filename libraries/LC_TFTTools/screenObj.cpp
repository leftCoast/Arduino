#include "screenObj.h"

screenObj* screen;  // Now we have a global screen so we don't have
                    // to pass the silly thing around all the time.

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// ****** screenObj ******

//Sets up the global screen object for us to use.
boolean initScreen(int inRotation) {

  screen = new screenObj();
  if (screen)
    return(screen->init(inRotation));
   else
   return(false);
}


screenObj::screenObj(void) 
:
Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, 0)
{
  ts = new TouchScreen(XP, YP, XM, YM, OHMS);
  xMapper  = new mapper(TS_MINX, TS_MAXX, width(), 0);
  yMapper  = new mapper(TS_MINY, TS_MAXY, height(), 0);
}


boolean screenObj::init(int inRotation) {

  uint16_t identifier;

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  reset();
  identifier = readID();
  if (identifier == 0x9325 || identifier == 0x9328) {
    begin(identifier); 
    setRotation(inRotation);
    return true;
  } 
  else
    return false;
}


void screenObj::fillRectGradient(word inX,word inY,word inXSize,word inYSize,word startColor16,word endColor16,boolean vertical) {

  colorObj startColor(startColor16);
  colorObj endColor(endColor16);

  fillRectGradient(inX,inY,inXSize,inYSize,&startColor,&endColor,vertical);
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

void screenObj::fillScreenGradient(word startColor16,word endColor16,boolean vertical) {

  fillRectGradient(0,0,_width,_height,startColor16,endColor16,vertical);
}


void screenObj::fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical) {

  fillRectGradient(0,0,_width,_height,startColor,endColor,vertical);
}


void screenObj::drawPixelInvert(word x,word y) {

  word color;

  
  color = readPixel(x,y);
  color = ~color;
  drawPixel(x,y,color);
}

void screenObj::frameRectInvert(TSPoint loc, word width,word height) {

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


TSPoint screenObj::getPoint(void) {

  TSPoint pt;

  pt = ts->getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pt.x = xMapper->Map(pt.x);
  pt.y = yMapper->Map(pt.y);
  switch (getRotation()) {
  case 0:
    break;
  case 1:
    swap(pt.x,pt.y);
    pt.x = pt.x;
    pt.y = height() - pt.y;
    break;
  case 2:
    pt.x = width() - pt.x;
    pt.y = height() - pt.y;
    break;
  case 3:
    swap(pt.x,pt.y);
    pt.x = width() - pt.x;
    pt.y = pt.y;
    break;
  }
  return(pt);
}


boolean screenObj::pressed(TSPoint inPt) { return(inPt.z > MINPRESSURE && inPt.z < MAXPRESSURE); }


