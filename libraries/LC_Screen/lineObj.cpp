#include "lineObj.h"

// Default is a black line across the default 16x16 rectangle.
lineObj::lineObj(void) { 

    color.setColor(&black);
    lnSize = 1;
    slope = positiveSlope;
}


lineObj::lineObj(int x1,int y1,int x2,int y2,colorObj* inColor) {

	setColor(inColor);
	setEnds(x1,y1,x2,y2);
}
	

lineObj::~lineObj(void) {  }

	
void lineObj::setColor(colorObj* inColor) {
  
  color.setColor(inColor);
  needRefresh = true;
}


void lineObj::setSize(byte inSize) {

  lnSize = inSize;
  needRefresh = true;
}


void lineObj::setEnds(int x1,int y1,int x2,int y2) {

  point point1;
  point point2;
  
  point1.x = x1;
  point1.y = y1;
  point2.x = x2;
  point2.y = y2;
  setEnds(&point1,&point2);
 }

 
void lineObj::setEnds(point* startPt,point* endPt) {

  setRect(startPt,endPt);
  if (startPt->x == endPt->x) slope = vertical;
  else if (startPt->y == endPt->y) slope = horizontal;
  else if ((x == startPt->x && y == startPt->y)||(x == endPt->x && y == endPt->y)) slope = negativeSlope;
  else slope = positiveSlope;
  needRefresh = true;
}


void lineObj::setEnds(rect* inRect,slopeType inSlope) {

  setRect(inRect);
  slope = inSlope;
  needRefresh = true;
}


void lineObj::drawSelf() {

  /*
  Serial.println("time to draw line.");
  Serial.print("x     : ");Serial.println(locX);
  Serial.print("y     : ");Serial.println(locY);
  Serial.print("width : ");Serial.println(width);
  Serial.print("height: ");Serial.println(height);
  */
  switch(slope) {
    case vertical : screen->drawVLine(x,y,height,&color); break;
    case positiveSlope : screen->drawLine(x,y-height,x+width,y,&color); break;
    case negativeSlope : screen->drawLine(x,y,x+width,y-height,&color); break;
    case horizontal : screen->drawHLine(x,y,width,&color); break;
  }
}


