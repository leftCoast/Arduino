#include "adafruit_684_Obj.h"


adafruit_684_Obj::adafruit_684_Obj(byte inCS,byte inRST)
  :displayObj(true,true,false,true,false) {

  theOLED = NULL;
  cs = inCS;
  rst = inRST;
}

adafruit_684_Obj::~adafruit_684_Obj(void) {

    if (theOLED) { 
      delete theOLED;
      theOLED = NULL;
    }
  }

  
bool adafruit_684_Obj::dispObjBegin(void) { 

  theOLED = new Adafruit_SSD1331(&SPI,cs, LC_DC, rst);
  if (theOLED!=NULL) {
    theOLED->begin();
    return true;
  }
  return false;
}

int adafruit_684_Obj::width(void)																							{ return theOLED->width(); }
int adafruit_684_Obj::height(void)																							{ return theOLED->height(); }
void adafruit_684_Obj::setRotation(byte inRotation)																	{ theOLED->setRotation(inRotation); }
void adafruit_684_Obj::setTextColor(colorObj* inColor)																{ theOLED->setTextColor(inColor->getColor16()); }
void adafruit_684_Obj::setTextColor(colorObj* tColor,colorObj* bColor)											{ theOLED->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_684_Obj::setTextSize(byte inSize)																			{ theOLED->setTextSize(inSize); }
void adafruit_684_Obj::setTextWrap(boolean wrap)																		{ theOLED->setTextWrap(wrap); }
void adafruit_684_Obj::setFont(const GFXfont* font)																	{ theOLED->setFont(font); }
void adafruit_684_Obj::setCursor(int x,int y)																			{ theOLED->setCursor(gX(x),gY(y)); }
void adafruit_684_Obj::drawText(char* inText)																			{ theOLED->print(inText); }
void adafruit_684_Obj::fillScreen(colorObj* inColor)																	{ theOLED->fillScreen(inColor->getColor16()); }
void adafruit_684_Obj::fillRect(int x,int y,int width,int height,colorObj* inColor)							{ theOLED->fillRect(gX(x),gY(y), width, height,inColor->getColor16()); }
void adafruit_684_Obj::drawRect(int x,int y,int width,int height,colorObj* inColor)							{ theOLED->drawRect(gX(x),gY(y), width, height,inColor->getColor16()); }
void adafruit_684_Obj::fillRect(rect* inRect,colorObj* inColor)													{ theOLED->fillRect(gX(inRect->x),gY(inRect->y), inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_684_Obj::drawRect(rect* inRect,colorObj* inColor)													{ theOLED->drawRect(gX(inRect->x),gY(inRect->y), inRect->width, inRect->height,inColor->getColor16()); }
void adafruit_684_Obj::fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theOLED->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_684_Obj::drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor)	{ theOLED->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_684_Obj::drawCircle(int x,int y,int diam, colorObj* inColor)										{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_684_Obj::fillCircle(int x,int y,int diam, colorObj* inColor)										{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_684_Obj::drawVLine(int x,int y,int height,colorObj* inColor)										{ theOLED->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void adafruit_684_Obj::drawHLine(int x,int y,int width,colorObj* inColor)										{ theOLED->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void adafruit_684_Obj::drawLine(int x,int y,int x2,int y2,colorObj* inColor)									{ theOLED->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
void adafruit_684_Obj::drawPixel(int x,int y,colorObj* pColor)														{ theOLED->drawPixel(gX(x),gY(y),pColor->getColor16()); }

/*
void	adafruit_684_Obj::blit(int locX,int locY,bitmap* inBitmap) {

	int		width;
	int		height;
	colorObj	aColor;
	unsigned int start = micros();
	unsigned int end;
	if (inBitmap) {
		width = inBitmap->getWidth();
		height = inBitmap->getHeight();
		theOLED->startWrite();
		for(int y=0;y<height;y++) {
			for(int x=0;x<width;x++) {
				aColor = inBitmap->getColor(x,y);
				//theOLED->drawPixel(x+locX,y+locY,aColor.getColor16());
				theOLED->writePixel(x+locX,y+locY,aColor.getColor16());
			}
		}
		theOLED->endWrite();
		end = micros();
		Serial.println(end-start);
	}
}
*/
 
