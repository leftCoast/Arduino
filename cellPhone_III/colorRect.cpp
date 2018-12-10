#include "colorRect.h"

colorRect::colorRect(int inLocX, int inLocY, word inWidth,word inHeight,int inset)
  : drawObj(inLocX,inLocY,inWidth,inHeight),
  colorObj(LC_WHITE) { mInset = inset; }
  
colorRect::~colorRect(void) { }

void colorRect::drawSelf(void) {

  colorObj  insetColor;
  
  screen->fillRect(x, y, width, height, (colorObj*)this);
  if (mInset) {
    insetColor.setColor(this);
    insetColor.blend(&black,50);
    screen->drawVLine(x,y,height-1,&insetColor);
    screen->drawHLine(x,y,width-1,&insetColor);
    insetColor.setColor(this);
    insetColor.blend(&white,50);
    screen->drawVLine(x+width,y+1,height-1,&insetColor);
    screen->drawHLine(x+1,y+height,width-1,&insetColor);
  }
}


void colorRect::setColor(byte r,byte g,byte b) {

  colorObj::setColor(r,g,b);
  needRefresh = true;
}


void colorRect::setColor(word color16) {

  colorObj::setColor(color16);
  needRefresh = true;
}


void colorRect::setColor(colorObj* inColor) {

  colorObj::setColor(inColor);
  needRefresh = true;
}
