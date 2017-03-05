#include "SSD_13XX_Obj.h"


SSD_13XX_Obj::SSD_13XX_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST)
  :displayObj(true,true,false,true,false) {

  theOLED = NULL;
  cs = inCS;
  dc = inDC;
  mosi = inMOSI;
  sclk = inSCLK;
  rst = inRST;
}

SSD_13XX_Obj::~SSD_13XX_Obj(void) {

    if (theOLED) { 
      delete theOLED;
      theOLED = NULL;
    }
  }

  
boolean SSD_13XX_Obj::dispObjBegin(void) { 

  theOLED = new SSD_13XX(cs,dc,rst,mosi,sclk);xs
  if (theOLED!=NULL) {
    theOLED->begin();
    return true;
  }
  return false;
}

void SSD_13XX_Obj::setRotation(byte inRotation)      { theOLED->setRotation(inRotation); }
void SSD_13XX_Obj::setTextColor(colorObj* inColor)   { theOLED->setTextColor(inColor->getColor16()); }
void SSD_13XX_Obj::setTextColor(colorObj* tColor,colorObj* bColor) { theOLED->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void SSD_13XX_Obj::setTextSize(byte inSize)          { theOLED->setTextSize(inSize); }
void SSD_13XX_Obj::setTextWrap(boolean wrap)         { theOLED->setTextWrap(wrap); }
void SSD_13XX_Obj::setFont(const GFXfont* font) 			{ theOLED->setFont(font); }
void SSD_13XX_Obj::setCursor(int inX,int inY)        { theOLED->setCursor(inX,inY); }
void SSD_13XX_Obj::drawText(char* inText)            { theOLED->print(inText); }
void SSD_13XX_Obj::fillScreen(colorObj* inColor)     { theOLED->fillScreen(inColor->getColor16()); }
void SSD_13XX_Obj::fillRect(int locX,int locY,word width,word height,colorObj* inColor) { theOLED->fillRect(locX, locY, width, height,inColor->getColor16()); }
void SSD_13XX_Obj::drawRect(int locX,int locY,word width,word height,colorObj* inColor) { theOLED->drawRect(locX, locY, width, height,inColor->getColor16()); }
void SSD_13XX_Obj::fillRoundRect(int locX,int locY,word width,word height,word radius,colorObj* inColor){ theOLED->fillRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void SSD_13XX_Obj::drawRoundRect(int locX,int locY,word width,word height,word radius,colorObj* inColor){ theOLED->drawRoundRect(locX,locY,width,height,radius,inColor->getColor16()); }
void SSD_13XX_Obj::drawCircle(int locX,int locY,word inDiam, colorObj* inColor) { drawRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void SSD_13XX_Obj::fillCircle(int locX,int locY,word inDiam, colorObj* inColor) { fillRoundRect(locX,locY,inDiam,inDiam,inDiam/2,inColor); }
void SSD_13XX_Obj::drawVLine(int locX,int locY,word height,colorObj* inColor) { theOLED->drawFastVLine(locX,locY,height,inColor->getColor16()); }
void SSD_13XX_Obj::drawHLine(int locX,int locY,word width,colorObj* inColor) { theOLED->drawFastHLine(locX,locY,width,inColor->getColor16()); }
void SSD_13XX_Obj::drawLine(int locX,int locY,int locX2,int locY2,colorObj* inColor) { theOLED->drawLine(locX,locY,locX2,locY2,inColor->getColor16()); }
void SSD_13XX_Obj::drawPixel(int locX,int locY,colorObj* pColor) { Serial.println("calling.."); theOLED->drawPixel(locX,locY,pColor->getColor16()); }


 
