#include "DFRobot_0669_Obj.h"

float		percBlack = 0;  
colorObj	gColor; 


colorObj* sDim(colorObj* inColor) {

	//return inColor;							// Debug. Keeps display on.
	if (percBlack<=0) return inColor;
	if (percBlack>=100) return &black;
	gColor.setColor(inColor);
	gColor.blend(&black,percBlack);
	return &gColor;
}
	
  
DFRobot_0669_Obj::DFRobot_0669_Obj(byte inCS,byte inRST)
  :displayObj(true,true,false,false,false) {

  theTFT = NULL;
  cs = inCS;
  rst = inRST;
}

DFRobot_0669_Obj::~DFRobot_0669_Obj(void) {

    if (theTFT) { 
      delete theTFT;
      theTFT = NULL;
    }
  }

  
bool DFRobot_0669_Obj::begin(void) { 

  theTFT = new ILI9488_t3(&SPI, cs, LC_DC, rst);
  if (theTFT!=NULL) {
    theTFT->begin();
    return true;
  }
  return false;
}


int	DFRobot_0669_Obj::width(void)																							{ return theTFT->width(); }
int	DFRobot_0669_Obj::height(void)																						{ return theTFT->height(); }
void	DFRobot_0669_Obj::setRotation(byte inRotation)																	{ theTFT->setRotation(inRotation); }
void	DFRobot_0669_Obj::setTextColor(colorObj* inColor)																{ theTFT->setTextColor(sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::setTextColor(colorObj* tColor,colorObj* bColor)											{ theTFT->setTextColor(sDim(tColor)->getColor16(),sDim(bColor)->getColor16()); }
void	DFRobot_0669_Obj::setTextSize(byte inSize)																		{ theTFT->setTextSize(inSize); }
void	DFRobot_0669_Obj::setTextWrap(boolean wrap)																		{ theTFT->setTextWrap(wrap); }

rect	DFRobot_0669_Obj::getTextRect(const char* inText) {

	rect bounds(0,0,0,0);
	int16_t	bX;
	int16_t	bY;
	uint16_t	bW;
	uint16_t	bH;
	
	bX = 0;
	bY = 0;
	bW = 0;
	bH = 0;
	theTFT->getTextBounds(inText,getCursorX(),getCursorY(),&bX,&bY,&bW,&bH);
	bounds.setRect(bX,bY,bW,bH);
	return bounds;
}

void	DFRobot_0669_Obj::setFont(const GFXfont* font)																	{ theTFT->setFont(font); }
void	DFRobot_0669_Obj::setCursor(int x,int y)																			{ theTFT->setCursor(gX(x),gY(y));  }
int	DFRobot_0669_Obj::getCursorX(void)																					{ return lX(theTFT->getCursorX()); }
int	DFRobot_0669_Obj::getCursorY(void)																					{ return lY(theTFT->getCursorY()); }
void	DFRobot_0669_Obj::drawText(const char* inText)																	{ theTFT->print(inText); }
void	DFRobot_0669_Obj::fillScreen(colorObj* inColor)																	{ theTFT->fillScreen(sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::fillRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->fillRect(gX(x),gY(y), width, height,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::drawRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->drawRect(gX(x),gY(y), width, height,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::fillRect(rect* inRect,colorObj* inColor)													{ theTFT->fillRect(gX(inRect->x),gY(inRect->y), inRect->width, inRect->height,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::drawRect(rect* inRect,colorObj* inColor)													{ theTFT->drawRect(gX(inRect->x),gY(inRect->y), inRect->width, inRect->height,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->fillRoundRect(gX(x),gY(y),width,height,radius,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->drawRoundRect(gX(x),gY(y),width,height,radius,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::drawCircle(int x,int y,int rad, colorObj* inColor)										{ theTFT->drawCircle(gX(x),gY(y),rad,inColor->getColor16()); }
void	DFRobot_0669_Obj::fillCircle(int x,int y,int rad, colorObj* inColor)										{ theTFT->fillCircle(gX(x),gY(y),rad,inColor->getColor16()); }
void	DFRobot_0669_Obj::drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor) 					{ theTFT->drawTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void	DFRobot_0669_Obj::fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor)					{ theTFT->fillTriangle(gP(pt0).x,gP(pt0).y,gP(pt1).x,gP(pt1).y,gP(pt2).x,gP(pt2).y,inColor->getColor16()); }
void	DFRobot_0669_Obj::drawVLine(int x,int y,int height,colorObj* inColor)									{ theTFT->drawFastVLine(gX(x),gY(y),height,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::drawHLine(int x,int y,int width,colorObj* inColor)										{ theTFT->drawFastHLine(gX(x),gY(y),width,sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::drawLine(int x,int y,int x2,int y2,colorObj* inColor)									{ theTFT->drawLine(gX(x),gY(y),gX(x2),gY(y2),sDim(inColor)->getColor16()); }
void	DFRobot_0669_Obj::drawPixel(int x,int y,colorObj* inColor)													{ theTFT->drawPixel(gX(x),gY(y),sDim(inColor)->getColor16()); }







 
