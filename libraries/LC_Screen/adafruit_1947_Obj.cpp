#include "adafruit_1947_Obj.h"


adafruit_1947_Obj::adafruit_1947_Obj(void)
:displayObj(true,true,true,true,false) {
    
	cTS			= NULL;
	theTFT		= NULL;
	cs				= ADA_1947_SHIELD_CS;
	rst			= ADA_1947_SHIELD_RST;
	lastTouch	= false;
	touchNum		= 0;
}


adafruit_1947_Obj::adafruit_1947_Obj(byte inCS,byte inRst)
  :displayObj(true,true,true,true,false) {

	cTS			= NULL;
	theTFT		= NULL;
	cs				= inCS;
	rst			= inRst;
	lastTouch	= false;
	touchNum		= 0;
}


adafruit_1947_Obj::~adafruit_1947_Obj(void) {

	if (theTFT!=NULL) {
		delete theTFT;
		theTFT = NULL;
	}
	if (cTS!=NULL) {
		delete cTS;
		cTS = NULL;
	}
}


bool adafruit_1947_Obj::dispObjBegin(void) { 

  theTFT = new Adafruit_ILI9341(cs,LC_DC,rst);
  //theTFT = new ILI9341_t3(cs,LC_DC,rst);
  if (theTFT!=NULL) {
    cTS = new Adafruit_FT6206();
    if (cTS!=NULL) {
    	theTFT->begin();										// Doesn't have fail mode here.
    	if (cTS->begin(40)) {								// 40 comes from example code.
    		return true;
    	}
    }
  }
  if (theTFT!=NULL) {
	 delete theTFT;
	 theTFT = NULL;
  }
  if (cTS!=NULL) {
    delete cTS;
    cTS = NULL;
  }
  return false;
}

int adafruit_1947_Obj::width(void)																							{ return theTFT->width(); }
int adafruit_1947_Obj::height(void)																							{ return theTFT->height(); }
void adafruit_1947_Obj::startWrite(void)																					{ theTFT->startWrite(); }
void adafruit_1947_Obj::endWrite(void)																						{ theTFT->endWrite(); }
//void adafruit_1947_Obj::startWrite(void) {  }
//void adafruit_1947_Obj::endWrite(void) {  }
void adafruit_1947_Obj::setRotation(byte inRotation)																	{ theTFT->setRotation(inRotation); }
void adafruit_1947_Obj::setTextColor(colorObj* inColor)																{ theTFT->setTextColor(inColor->getColor16()); }
void adafruit_1947_Obj::setTextColor(colorObj* tColor,colorObj* bColor) 										{ theTFT->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_1947_Obj::setTextSize(byte inSize)																		{ theTFT->setTextSize(inSize); }
void adafruit_1947_Obj::setTextWrap(boolean wrap)																		{ theTFT->setTextWrap(wrap); }
void adafruit_1947_Obj::setFont(const GFXfont* font)																	{ theTFT->setFont(font); }
//void adafruit_1947_Obj::setFont(const GFXfont* font)																	{ theTFT->setFontAdafruit(); }
void adafruit_1947_Obj::setCursor(int x,int y)																			{ theTFT->setCursor(gX(x),gY(y)); }
void adafruit_1947_Obj::drawText(char* inText)																			{ theTFT->print(inText); }
void adafruit_1947_Obj::fillScreen(colorObj* inColor)																	{ theTFT->fillScreen(inColor->getColor16()); }
void adafruit_1947_Obj::fillRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->fillRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_1947_Obj::drawRect(int x,int y,int width,int height,colorObj* inColor)						{ theTFT->drawRect(gX(x),gY(y),width, height,inColor->getColor16()); }
void adafruit_1947_Obj::fillRect(rect* inRect,colorObj* inColor)													{ theTFT->fillRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_1947_Obj::drawRect(rect* inRect,colorObj* inColor)													{ theTFT->drawRect(gX(inRect->x),gY(inRect->y),inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_1947_Obj::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1947_Obj::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theTFT->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1947_Obj::drawCircle(int x,int y,int diam, colorObj* inColor)									{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1947_Obj::drawCircleHelper(int x,int y,int rad,byte corner,colorObj* inColor)				{ theTFT->drawCircleHelper(gX(x),gY(y),rad,corner,inColor->getColor16()); }
void adafruit_1947_Obj::fillCircle(int x,int y,int diam, colorObj* inColor)									{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1947_Obj::drawVLine(int x,int y,int height,colorObj* inColor)									{ theTFT->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void adafruit_1947_Obj::drawHLine(int x,int y,int width,colorObj* inColor)										{ theTFT->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void adafruit_1947_Obj::drawLine(int x,int y,int x2,int y2,colorObj* inColor)									{ theTFT->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
 
  
void adafruit_1947_Obj::drawPixel(int x,int y,colorObj* pColor) { theTFT->drawPixel(gX(x),gY(y),pColor->getColor16()); }
	 
point adafruit_1947_Obj::getPoint(void) {

  TS_Point adaPoint;
  point		lcPoint;

  adaPoint = cTS->getPoint();
  switch (theTFT->getRotation()) {
  case INV_PORTRAIT:
    break;
  case INV_LANDSCAPE:
    swap(adaPoint.x,adaPoint.y);
    adaPoint.x = adaPoint.x;
    adaPoint.y = theTFT->height() - adaPoint.y;
    break;
  case PORTRAIT:
    adaPoint.x = theTFT->width() - adaPoint.x;
    adaPoint.y = theTFT->height() - adaPoint.y;
    break;
  case LANDSCAPE:
    swap(adaPoint.x,adaPoint.y);
    adaPoint.x = theTFT->width() - adaPoint.x;
    adaPoint.y = adaPoint.y;
    break;
  }
  lcPoint.x = adaPoint.x;
  lcPoint.y = adaPoint.y;
  return lcPoint;
}


bool adafruit_1947_Obj::touched(void) { return cTS->touched(); }

