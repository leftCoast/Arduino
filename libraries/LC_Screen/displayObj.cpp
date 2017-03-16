 #include "displayObj.h"

  
  displayObj::displayObj(boolean inGraphic,boolean inColor,boolean inTouch,boolean inSD,boolean inReadable) {

    graphic = inGraphic;
    color = inColor;
    hasTouch = inTouch;
    hasSD = inSD;
    readable = inReadable;
    offsetX = 0;
  	offsetY = 0;
  }
  
  
void displayObj::pushOffset(int x, int y) {

	offsetX = offsetX + x;
	offsetY = offsetY + y;
}


void displayObj::popOffset(int x, int y) {

	offsetX = offsetX - x;
	offsetY = offsetY - y;
}

  
int displayObj::gX(int x) { return x + offsetX; }

int displayObj::gY(int y) { return y + offsetY; }
	
	
point	displayObj::lP(point gP) {

	point aPt;
	
	aPt.x = gP.x -  offsetX;
	aPt.y = gP.y -  offsetY;
	return aPt;
}


boolean displayObj::dispObjBegin(void) { return false; }
void displayObj::setRotation(byte inRotation) { }
void displayObj::setTextColor(colorObj* inColor) { }
void displayObj::setTextColor(colorObj* tColor,colorObj* bColor) { }
void displayObj::setTextSize(byte inSize) { }
void displayObj::setTextWrap(boolean inWrap) { }
void displayObj::setFont(const GFXfont* font) { }
void displayObj::setCursor(int inX,int inY) { }
void displayObj::drawText(char* inText) { }
void displayObj::fillScreen(colorObj* inColor) { }
void displayObj::fillRect(int locX,int locY,word width,word height,colorObj* inColor) { }
void displayObj::drawRect(int locX,int locY,word width,word  height,colorObj* inColor) { }
void displayObj::fillRectGradient(int inX,int inY,word inXSize,word inYSize,colorObj* startColor,colorObj* endColor,boolean vertical) { }
void displayObj::fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical) { }
void displayObj::fillRoundRect(int locX,int locY,word width,word height,word radius,colorObj* inColor){ }
void displayObj::drawRoundRect(int locX,int locY,word width,word height,word radius,colorObj* inColor){ }
void displayObj::drawCircle(int locX,int locY,word inDiam, colorObj* inColor){ }
void displayObj::fillCircle(int locX,int locY,word inDiam, colorObj* inColor){ }
void displayObj::drawVLine(int locX,int locY,word height,colorObj* inColor) { }
void displayObj::drawHLine(int locX,int locY,word width,colorObj* inColor) { }
void displayObj::drawLine(int locX,int locY,int locX2,int locY2,colorObj* inColor) { }
void displayObj::drawPixel(int locX,int locY,colorObj* pColor) { }
void displayObj::drawPixelInvert(int x,int y) { }
void displayObj::frameRectInvert(int x,int y,word width,word height) { }

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

