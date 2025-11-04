#include <drawDelete.h>  // <<=== THIS HAS GOT TO GO!!

drawDelete::drawDelete(rect* inRect)
  : stampObj(inRect) { }

drawDelete::~drawDelete(void) { }


void drawDelete::setColors(colorObj* foreColor,colorObj* backColor) {

  mForeColor = *foreColor;
  mBackColor = *backColor;
}

          
void drawDelete::stamp(void) {

  point arrowPoint[6];
  int   top;
  int   middle;
  int   bottom;
  int   left;
  int   bend;
  int   right;

  top = round(y + (.25 * height));
  middle = round(y + (height/2));
  bottom = round(y + (.75 * height));
  left = x;
  bend = round(x + (.3 * width));
  right = x+width-1;

  screen->fillRect(this,&mBackColor);
  
  arrowPoint[0].x = left; arrowPoint[0].y = middle;
  arrowPoint[1].x = bend; arrowPoint[1].y = top;
  arrowPoint[2].x = right; arrowPoint[2].y = top;
  arrowPoint[3].x = right; arrowPoint[3].y = bottom;
  arrowPoint[4].x = bend; arrowPoint[4].y = bottom;
  arrowPoint[5].x = left; arrowPoint[5].y = middle;

  for (byte i=0;i<5;i++) {
    screen->drawLine(arrowPoint[i].x,arrowPoint[i].y,arrowPoint[i+1].x,arrowPoint[i+1].y,&mForeColor);
  }
  
  arrowPoint[0].x = left+1; arrowPoint[0].y = middle;
  arrowPoint[1].x = bend; arrowPoint[1].y = top+1;
  arrowPoint[2].x = right-1; arrowPoint[2].y = top+1;
  arrowPoint[3].x = right-1; arrowPoint[3].y = bottom-1;
  arrowPoint[4].x = bend+1; arrowPoint[4].y = bottom-1;
  arrowPoint[5].x = left+1; arrowPoint[5].y = middle;

  for (byte i=0;i<5;i++) {
    screen->drawLine(arrowPoint[i].x,arrowPoint[i].y,arrowPoint[i+1].x,arrowPoint[i+1].y,&mForeColor);
  }
}