#include "movingObj.h"

movingObj::movingObj(int inX, int inY, word inWidth,word inHeight) 
  :drawObj(inX,inY,inWidth,inHeight) {
    backColor.setColor((word)BLACK);
    oldX = scrX();
    oldY = scrY(); 
  }

void movingObj::setBackColor(colorObj* inBackColor) { backColor.setColor(inBackColor); }


void movingObj::setLocation(int inX, int inY) {

  oldX = scrX();
  oldY = scrY();
  drawObj::setLocation(inX,inY);
}


void movingObj::eraseSelf(void) { screen->fillRect(oldX,oldY,width,height,&backColor); }


void movingObj::drawSelf(void) {
  eraseSelf();
  screen->fillRect(scrX(), scrY(), width, height, &white);
}

int movingObj::middleX(void) { return x+width/2; }

