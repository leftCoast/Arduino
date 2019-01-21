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


boolean bricks(void) {

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
  : movingObj(inX,inY,BRICK_W,BRICK_H) {

    homeX = inX;
    homeY = inY;
    brickList[brickIndex++] = this;
  }


brickObj::~brickObj(void) {  }


void brickObj::setColor(colorObj* inColor) {
  
    brickColor.setColor(inColor);
    hilight.setColor(inColor);
    hilight.blend(&white,HILIGHT_PRCT);
    shadow.setColor(inColor);
    shadow.blend(&black,SHADOW_PRCT);
  }


void brickObj::hit(void) {

  screen->fillRect(x,y,width,height,&white);
  delay(10);
  eraseSelf();            // Erased, from exsistance.
  drawObj::setLocation(-100,-100); // We're outta' here!
}


void brickObj::reset(void) { drawObj::setLocation(homeX,homeY); }


void brickObj::drawSelf(void) {

  if (x>0 && y>0) {
    screen->fillRect(x,y,width,height,&brickColor);
    screen->drawHLine(x,y,width-1,&hilight);
    screen->drawVLine(x,y,height-1,&hilight);
    screen->drawHLine(x+1,y+height-1,width-1,&shadow);
    screen->drawVLine(x+width-1,y+1,height-2,&shadow);
  } 
}
