#include "adafruit_1431_Obj.h"


adafruit_1431_Obj::adafruit_1431_Obj(byte inCS,byte inRST)
  :displayObj(true,true,false,true,false) {

  theOLED = NULL;
  cs = inCS;
  rst = inRST;
}

adafruit_1431_Obj::~adafruit_1431_Obj(void) {

    if (theOLED) { 
      delete theOLED;
      theOLED = NULL;
    }
  }

  
boolean adafruit_1431_Obj::dispObjBegin(void) { 

  theOLED = new Adafruit_SSD1351(cs, LC_DC, rst);
  if (theOLED!=NULL) {
    theOLED->begin();
    return true;
  }
  return false;
}

void adafruit_1431_Obj::setRotation(byte inRotation)      							{ theOLED->setRotation(inRotation); }
void adafruit_1431_Obj::setTextColor(colorObj* inColor)   							{ theOLED->setTextColor(inColor->getColor16()); }
void adafruit_1431_Obj::setTextColor(colorObj* tColor,colorObj* bColor) { theOLED->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_1431_Obj::setTextSize(byte inSize)          							{ theOLED->setTextSize(inSize); }
void adafruit_1431_Obj::setTextWrap(boolean wrap)         							{ theOLED->setTextWrap(wrap); }
void adafruit_1431_Obj::setFont(const GFXfont* font) 										{ theOLED->setFont(font); }
void adafruit_1431_Obj::setCursor(int x,int y)        									{ theOLED->setCursor(gX(x),gY(y)); }
void adafruit_1431_Obj::drawText(char* inText)            							{ theOLED->print(inText); }
void adafruit_1431_Obj::fillScreen(colorObj* inColor)     							{ theOLED->fillScreen(inColor->getColor16()); }
void adafruit_1431_Obj::fillRect(int x,int y,word width,word height,colorObj* inColor) 									{ theOLED->fillRect(gX(x),gY(y), width, height,inColor->getColor16()); }
void adafruit_1431_Obj::drawRect(int x,int y,word width,word height,colorObj* inColor)									{ theOLED->drawRect(gX(x),gY(y), width, height,inColor->getColor16()); }
void adafruit_1431_Obj::fillRoundRect(int x,int y,word width,word height,word radius,colorObj* inColor)	{ theOLED->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1431_Obj::drawRoundRect(int x,int y,word width,word height,word radius,colorObj* inColor)	{ theOLED->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void adafruit_1431_Obj::drawCircle(int x,int y,word diam, colorObj* inColor) 														{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1431_Obj::fillCircle(int x,int y,word diam, colorObj* inColor) 														{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void adafruit_1431_Obj::drawVLine(int x,int y,word height,colorObj* inColor) 														{ theOLED->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void adafruit_1431_Obj::drawHLine(int x,int y,word width,colorObj* inColor) 														{ theOLED->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void adafruit_1431_Obj::drawLine(int x,int y,int x2,int y2,colorObj* inColor) 													{ theOLED->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
void adafruit_1431_Obj::drawPixel(int x,int y,colorObj* pColor) 																				{ theOLED->drawPixel(gX(x),gY(y),pColor->getColor16()); }


 
