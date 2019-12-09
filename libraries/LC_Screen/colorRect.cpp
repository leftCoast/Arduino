#include "colorRect.h"


colorRect::colorRect(void)
	: drawObj(),colorObj() {
	
	setInset(0);
}


colorRect::colorRect(rect* inRect,colorObj* inColor,int inset)
	: drawObj(inRect),colorObj() {
	
	setColor(inColor);
	setInset(inset);
}
	
	
colorRect::colorRect(int inLocX, int inLocY, int inWidth,int inHeight,int inset)
  : drawObj(inLocX,inLocY,inWidth,inHeight),
  colorObj(LC_WHITE) { setInset(inset); }
  
  
colorRect::~colorRect(void) { }


void colorRect::setInset(int inset) {

    mInset = inset;
    needRefresh = true;
}


void colorRect::drawSelf(void) {

  colorObj  insetTLColor;
  colorObj  insetBRColor;
  int       j;
  
  screen->fillRect(x, y, width, height, (colorObj*)this);
  if (mInset) {
    insetTLColor = mixColors(&black,50);
    insetBRColor = mixColors(&white,50);
    j = 1;
    for (int i=0;i<mInset;i++) {
      screen->drawVLine(x+i,y+i,height-j,&insetTLColor);
      screen->drawHLine(x+i,y+i,width-j,&insetTLColor);
      screen->drawVLine(x+width-i,y+j,height-j,&insetBRColor);
      screen->drawHLine(x+j,y+height-i,width-j,&insetBRColor);
      j++;
    }
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
