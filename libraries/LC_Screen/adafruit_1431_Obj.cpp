#include "adafruit_1431_Obj.h"


adafruit_1431_Obj::adafruit_1431_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST)
  :displayObj(true,true,false,true,false) {

  theOLED = NULL;
  cs = inCS;
  dc = inDC;
  mosi = inMOSI;
  sclk = inSCLK;
  rst = inRST;
}

adafruit_1431_Obj::~adafruit_1431_Obj(void) {

    if (theOLED) { 
      delete theOLED;
      theOLED = NULL;
    }
  }

  
boolean adafruit_1431_Obj::dispObjBegin(void) { 

  theOLED = new Adafruit_SSD1351(cs, dc, mosi, sclk, rst);
  if (theOLED!=NULL) {
    theOLED->begin();
    return true;
  }
  return false;
}

void adafruit_1431_Obj::setRotation(byte inRotation)      { theOLED->setRotation(inRotation); }
void adafruit_1431_Obj::setTextColor(colorObj* inColor)   { theOLED->setTextColor(inColor->getColor16()); }
void adafruit_1431_Obj::setTextColor(colorObj* tColor,colorObj* bColor) { theOLED->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_1431_Obj::setTextSize(byte inSize)          { theOLED->setTextSize(inSize); }
void adafruit_1431_Obj::setCursor(int inX,int inY)        { theOLED->setCursor(inX,inY); }
void adafruit_1431_Obj::drawText(char* inText)            { theOLED->print(inText); }
void adafruit_1431_Obj::fillScreen(colorObj* inColor)     { theOLED->fillScreen(inColor->getColor16()); }
void adafruit_1431_Obj::fillRect(word locX,word locY,word width,word height,colorObj* inColor) { theOLED->fillRect(locX, locY, width, height,inColor->getColor16()); }
void adafruit_1431_Obj::drawRect(word locX,word locY,word width,word height,colorObj* inColor) { theOLED->drawRect(locX, locY, width, height,inColor->getColor16()); }
void adafruit_1431_Obj::fillRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ theOLED->fillRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void adafruit_1431_Obj::drawRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor){ theOLED->drawRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void adafruit_1431_Obj::drawCircle(word locX,word locY,word inDiam, colorObj* inColor) { drawRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void adafruit_1431_Obj::fillCircle(word locX,word locY,word inDiam, colorObj* inColor) { fillRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void adafruit_1431_Obj::drawVLine(word locX,word locY,word height,colorObj* inColor) { theOLED->drawFastVLine(locX,locY,height,inColor->getColor16()); }
void adafruit_1431_Obj::drawHLine(word locX,word locY,word width,colorObj* inColor) { theOLED->drawFastHLine(locX,locY,width,inColor->getColor16()); }
void adafruit_1431_Obj::drawLine(word locX,word locY,word locX2,word locY2,colorObj* inColor) { theOLED->drawLine(locX,locY,locX2,locY2,inColor->getColor16()); }

void adafruit_1431_Obj::drawPixel(word x,word y,colorObj* pColor) { theOLED->drawPixel(x,y,pColor->getColor16()); }


 
