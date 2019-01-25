#include "brickObj.h"

brickObj* brickList[TOTAL_BRICKS];
int       brickIndex = 0;


void resetBricks(void) {

  brickObj* aBrick;
  
  for(int i=0;i<TOTAL_BRICKS;i++) {
    aBrick = brickList[i];
    if (aBrick) aBrick->reset();
  }
}


bool bricks(void) {

  brickObj* aBrick;

  for(int i=0;i<TOTAL_BRICKS;i++) {
    aBrick = brickList[i];
    if (aBrick) {
      if (aBrick->x>0) return true;
    }
  }
  return false;
}



brickObj::brickObj(int inX,int inY)
  : movingObj(inX,inY,BRICK_W,BRICK_H),
  bmpPipe() {

    homeX = inX;
    homeY = inY;
    brickList[brickIndex++] = this;
  }


brickObj::~brickObj(void) {  }


void brickObj::setColor(char* path) {

  rect  ourSource;
  
  ourSource.setRect(0,0,width,height);
  openPipe(path);
  setSourceRect(ourSource);
}


void brickObj::hit(void) {

  screen->fillRect(x,y,width,height,&white);  // A flash!
  delay(10);                                  // Nasty delay.
  eraseSelf();                                // Erased, from exsistance.
  drawObj::setLocation(-100,-100);            // We're outta' here!
}


void brickObj::reset(void) {  drawObj::setLocation(homeX,homeY); }


void brickObj::drawSelf(void) {
  
  if (x>0 && y>0) {
    drawBitmap(x,y);
  } 
}
