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


void adafruit_1431_Obj::setTextColor(colorObj* inColor)   { theOLED->setTextColor(inColor->getColor16()); }
void adafruit_1431_Obj::setTextColor(colorObj* tColor,colorObj* bColor) { theOLED->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void adafruit_1431_Obj::setTextSize(byte inSize)          { theOLED->setTextSize(inSize); }
void adafruit_1431_Obj::setCursor(int inX,int inY)        { theOLED->setCursor(inX,inY); }
void adafruit_1431_Obj::drawText(char* inText)            { theOLED->print(inText); }
void adafruit_1431_Obj::fillScreen(colorObj* inColor)     { theOLED->fillScreen(inColor->getColor16()); }
void adafruit_1431_Obj::fillRect(word locX,word locY,word width,word height,colorObj* inColor) { theOLED->fillRect(locX, locY, width, height,inColor->getColor16()); }
void adafruit_1431_Obj::drawRect(word locX,word locY,word width,word height,colorObj* inColor) { theOLED->drawRect(locX, locY, width, height,inColor->getColor16()); }
void adafruit_1431_Obj::drawPixel(word x,word y,colorObj* pColor) { theOLED->drawPixel(x,y,pColor->getColor16()); }


 
