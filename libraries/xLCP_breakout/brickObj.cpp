#include "brickObj.h"

brickObj* brickList[TOTAL_BRICKS];  // Move these two into.. The breakout class?
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
	: drawObj(inX,inY,BRICK_W,BRICK_H) {

	homeX = inX;
	homeY = inY;
	brickList[brickIndex++] = this;
}


brickObj::~brickObj(void) {  }


void brickObj::setColor(bitmap* inBitmap) { ourBMap = inBitmap; }


void brickObj::hit(void) {

  screen->fillRect(x,y,width,height,&white);	// A flash!
  soundHitBrick();
  delay(20);											// Nasty delay.
  screen->fillRect(x,y,width,height,this);	// Erased, from exsistance.
  drawObj::setLocation(-100,-100);				// We're outta' here!
}


void brickObj::reset(void) {  drawObj::setLocation(homeX,homeY); }


void brickObj::drawSelf(void) {
  
  if (x>0 && y>0) {
    screen->blit(x,y,ourBMap);
  }
}
