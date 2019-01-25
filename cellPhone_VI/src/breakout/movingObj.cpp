#include "movingObj.h"

movingObj::movingObj(int inX, int inY, word inWidth,word inHeight) 
  :drawObj(inX,inY,inWidth,inHeight,true) {
    backColor.setColor(LC_BLACK);
    oldX = x;
    oldY = y; 
  }


movingObj::~movingObj(void) {  }


void movingObj::setBackColor(colorObj* inBackColor) { backColor.setColor(inBackColor); }


void movingObj::setLocation(int inX, int inY) {

  oldX = x;
  oldY = y; 
  drawObj::setLocation(inX,inY);
}


void movingObj::eraseSelf(void) { screen->fillRect(oldX,oldY,width,height,&backColor); }


void movingObj::drawSelf(void) {
  eraseSelf();
  screen->fillRect(x,y, width, height, &white);
}

int movingObj::middleX(void) { return x+width/2; }
