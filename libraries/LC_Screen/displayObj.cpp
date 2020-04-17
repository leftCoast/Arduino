 #include "displayObj.h"

  
displayObj::displayObj(bool inGraphic,bool inColor,bool inTouch,bool inSD,bool inReadable) {

	graphic = inGraphic;
	color = inColor;
	hasTouch = inTouch;
	hasSD = inSD;
	readable = inReadable;
	offsetX = 0;
	offsetY = 0;
  }
  

displayObj::~displayObj(void) { }

  
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

int  displayObj::width(void) {  }
int  displayObj::height(void) {  }
void displayObj::startWrite(void) {  }
void displayObj::endWrite(void) {  }
bool displayObj::dispObjBegin(void) { return false; }
void displayObj::setRotation(byte inRotation) { }
void displayObj::setTextColor(colorObj* inColor) { }
void displayObj::setTextColor(colorObj* tColor,colorObj* bColor) { }
void displayObj::setTextSize(byte inSize) { }
void displayObj::setTextWrap(bool inWrap) { }
void displayObj::setFont(const GFXfont* font) { }
void displayObj::setCursor(int inX,int inY) { }
void displayObj::drawText(char* inText) { }
void displayObj::drawChar(int x,int y,char inChar,colorObj* fColor,colorObj* bColor,int size) { }
void displayObj::fillScreen(colorObj* inColor) {  }
void displayObj::fillRect(int locX,int locY,int width,int height,colorObj* inColor) { }
void displayObj::fillRect(rect* inRect,colorObj* inColor) { }
void displayObj::drawRect(int locX,int locY,int width,int  height,colorObj* inColor) { }
void displayObj::drawRect(rect* inRect,colorObj* inColor) { }
void displayObj::fillRectGradient(int inX,int inY,int inXSize,int inYSize,colorObj* startColor,colorObj* endColor,bool vertical) { }
void displayObj::fillScreenGradient(colorObj* startColor,colorObj* endColor,bool vertical) { }
void displayObj::fillRoundRect(int locX,int locY,int width,int height,int radius,colorObj* inColor){ }
void displayObj::drawRoundRect(int locX,int locY,int width,int height,int radius,colorObj* inColor){ }
void displayObj::fillRoundRect(rect* inRect,int radius,colorObj* inColor){ fillRoundRect(inRect->x,inRect->y,inRect->width,inRect->height,radius,inColor); }
void displayObj::drawRoundRect(rect* inRect,int radius,colorObj* inColor){ drawRoundRect(inRect->x,inRect->y,inRect->width,inRect->height,radius,inColor); }
void displayObj::drawCircle(int locX,int locY,int inDiam, colorObj* inColor){ }
void displayObj::drawCircleHelper(int locX,int locY,int inRad,byte corner,colorObj* inColor) { }
void displayObj::fillCircle(int locX,int locY,int inDiam, colorObj* inColor){ }
void displayObj::drawVLine(int locX,int locY,int height,colorObj* inColor) { }
void displayObj::drawHLine(int locX,int locY,int width,colorObj* inColor) { }
void displayObj::drawLine(int locX,int locY,int locX2,int locY2,colorObj* inColor) { }
void displayObj::drawLine(point* startPt,point* endPt,colorObj* inColor) { drawLine(startPt->x,startPt->y,endPt->x,endPt->y,inColor); }
void displayObj::drawPixel(int locX,int locY,colorObj* pColor) { }
void displayObj::drawPixelInvert(int x,int y) { }
void displayObj::frameRectInvert(int x,int y,int width,int height) { }

void displayObj::blit(int locX,int locY,bitmap* inBitmap) {

	int		width;
	int		height;
	colorObj	aColor;
	
	if (inBitmap) {
		width = inBitmap->getWidth();
		height = inBitmap->getHeight();
		for(int y=0;y<height;y++) {
			for(int x=0;x<width;x++) {
				aColor = inBitmap->getColor(x,y);
				drawPixel(x+locX,y+locY,&aColor);
			}
		}
	}
}


point   displayObj::getPoint(void) {
  
  point defPnt;
  
  defPnt.x = 0;
  defPnt.y = 0;
  return defPnt;
}

bool displayObj::touched(void) {return false; }
  
bool displayObj::isGraphic(void) { return graphic; }
bool displayObj::isColor(void) { return color; }
bool displayObj::hasTouchScreen(void) { return hasTouch; }
bool displayObj::hadSDDrive(void) { return hasSD; }
bool displayObj::canRead(void) { return readable; }

