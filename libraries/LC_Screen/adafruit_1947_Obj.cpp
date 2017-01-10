#include "adafruit_1947_Obj.h"

adafruit_1947_Obj::adafruit_1947_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST,byte inMISO)
  :displayObj(true,true,true,true,false) {

  cTS = NULL;
  theTFT = NULL;
  cs = inCS;
  dc = inDC;
  mosi = inMOSI;
  sclk = inSCLK;
  rst = inRST;
  miso = inMISO;
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


boolean adafruit_1947_Obj::dispObjBegin(void) { 

  theTFT = new Adafruit_ILI9341(cs,dc,mosi,sclk,rst,miso);
  if (theTFT!=NULL) {
    cTS = new Adafruit_FT6206();
    if (cTS!=NULL) {
    	theTFT->begin();	// Doesn't have fail mode here.
    	if (cTS->begin(40)) {	// 40 comes from example code.
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

void adafruit_1947_Obj::setRotation(byte inRotation)      { theTFT->setRotation(inRotation); }
void adafruit_1947_Obj::setTextColor(colorObj* inColor)   { theTFT->setTextColor(inColor->getColor16()); }
void adafruit_1947_Obj::setTextColor(colorObj* tColor,colorObj* bColor) { theTFT->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_1947_Obj::setTextSize(byte inSize)          { theTFT->setTextSize(inSize); }
void adafruit_1947_Obj::setTextWrap(boolean wrap) { theTFT->setTextWrap(wrap); }
void adafruit_1947_Obj::setCursor(word inX,word inY) { theTFT->setCursor(inX,inY); }
void adafruit_1947_Obj::drawText(char* inText)            { theTFT->print(inText); }
void adafruit_1947_Obj::fillScreen(colorObj* inColor)     { theTFT->fillScreen(inColor->getColor16()); }
void adafruit_1947_Obj::fillRect(word locX,word locY,word width,word height,colorObj* inColor) { theTFT->fillRect(locX, locY, width, height,inColor->getColor16()); }
void adafruit_1947_Obj::drawRect(word locX,word locY,word width,word height,colorObj* inColor) { theTFT->drawRect(locX, locY, width, height,inColor->getColor16()); }
void adafruit_1947_Obj::fillRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ theTFT->fillRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void adafruit_1947_Obj::drawRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ theTFT->drawRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void adafruit_1947_Obj::drawCircle(word locX,word locY,word inDiam, colorObj* inColor) { drawRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void adafruit_1947_Obj::fillCircle(word locX,word locY,word inDiam, colorObj* inColor) { fillRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void adafruit_1947_Obj::drawVLine(word locX,word locY,word height,colorObj* inColor) { theTFT->drawFastVLine(locX,locY,height,inColor->getColor16()); }
void adafruit_1947_Obj::drawHLine(word locX,word locY,word width,colorObj* inColor) { theTFT->drawFastHLine(locX,locY,width,inColor->getColor16()); }
void adafruit_1947_Obj::drawLine(word locX,word locY,word locX2,word locY2,colorObj* inColor) { theTFT->drawLine(locX,locY,locX2,locY2,inColor->getColor16()); }
 
  
void adafruit_1947_Obj::drawPixel(word x,word y,colorObj* pColor) { theTFT->drawPixel(x,y,pColor->getColor16()); }
	 
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


boolean adafruit_1947_Obj::touched(void) { return cTS->touched(); }

 
