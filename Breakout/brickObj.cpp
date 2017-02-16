#include "brickObj.h"

brickObj* brickList[TOTAL_BRICKS];
int       brickIndex = 0;

brickObj::brickObj(int inX,int inY)
  : movingObj(inX,inY,BRICK_W,BRICK_H) {

    homeX = inX;
    homeY = inY;
    brickList[brickIndex++] = this;
  }


void brickObj::setColor(colorObj* inColor) { brickColor.setColor(inColor); }


void brickObj::hit(void) {

  screen->fillRect(x,y,width,height,&white);
  delay(10);
  eraseSelf();            // Erased, from exsistance.
  setLocation(-100,-100); // We're outta' here!
}


void brickObj::reset(void) { setLocation(homeX,homeY); }


void brickObj::drawSelf(void) {

  if (x>0 && y>0) {
    screen->fillRect(x,y,width,height,&brickColor);
    screen->drawRect(x,y,width,height,&backColor);
  } 
}

