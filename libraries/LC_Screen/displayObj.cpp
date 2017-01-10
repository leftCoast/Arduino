 #include "displayObj.h"

  
  displayObj::displayObj(boolean inGraphic,boolean inColor,boolean inTouch,boolean inSD,boolean inReadable) {

    graphic = inGraphic;
    color = inColor;
    hasTouch = inTouch;
    hasSD = inSD;
    readable = inReadable;
  }

boolean displayObj::dispObjBegin(void) { return false; }
void displayObj::setRotation(byte inRotation) { }
void displayObj::setTextColor(colorObj* inColor) { }
void displayObj::setTextColor(colorObj* tColor,colorObj* bColor) { }
void displayObj::setTextSize(byte inSize) { }
void displayObj::setTextWrap(boolean inWrap) { }
void displayObj::setCursor(word inX,word inY) { }
void displayObj::drawText(char* inText) { }
void displayObj::fillScreen(colorObj* inColor) { }
void displayObj::fillRect(word locX,word locY,word width,word height,colorObj* inColor) { }
void displayObj::drawRect(word locX,word locY,word width,word  height,colorObj* inColor) { }
void displayObj::fillRectGradient(word inX,word inY,word inXSize,word inYSize,colorObj* startColor,colorObj* endColor,boolean vertical) { }
void displayObj::fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical) { }
void displayObj::fillRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ }
void displayObj::drawRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ }
void displayObj::drawCircle(word locX,word locY,word inDiam, colorObj* inColor){ }
void displayObj::fillCircle(word locX,word locY,word inDiam, colorObj* inColor){ }
void displayObj::drawVLine(word locX,word locY,word locY2,colorObj* inColor) { }
void displayObj::drawHLine(word locX,word locY,word locX2,colorObj* inColor) { }
void displayObj::drawLine(word locX,word locY,word locX2,word locY2,colorObj* inColor) { }
void displayObj::drawPixel(word x,word y,colorObj* pColor) { }
void displayObj::drawPixelInvert(word x,word y) { }
void displayObj::frameRectInvert(word x,word y,word width,word height) { }

point   displayObj::getPoint(void) {
  
  point defPnt;
  
  defPnt.x = 0;
  defPnt.y = 0;
  return defPnt;
}

boolean displayObj::touched(void) {return false; }
  
bool displayObj::isGraphic(void) { return graphic; }
bool displayObj::isColor(void) { return color; }
bool displayObj::hasTouchScreen(void) { return hasTouch; }
bool displayObj::hadSDDrive(void) { return hasSD; }
bool displayObj::canRead(void) { return readable; }

