#include "SSD_13XX_Obj.h"


SSD_13XX_Obj::SSD_13XX_Obj(byte inCS,byte inRST)
  :displayObj(true,true,false,true,false) {

  theOLED = NULL;
  cs = inCS;
  rst = inRST;
}

SSD_13XX_Obj::~SSD_13XX_Obj(void) {

    if (theOLED) { 
      delete theOLED;
      theOLED = NULL;
    }
  }

  
boolean SSD_13XX_Obj::dispObjBegin(void) { 

  theOLED = new SSD_13XX(cs,LC_DC,rst);	// OLED MUST have reset. Won't work otherwise.
  if (theOLED!=NULL) {
    theOLED->begin();
    return true;
  }
  return false;
}

void SSD_13XX_Obj::setRotation(byte inRotation)      								{ theOLED->setRotation(inRotation); }
void SSD_13XX_Obj::setTextColor(colorObj* inColor)									{ theOLED->setTextColor(inColor->getColor16()); }
void SSD_13XX_Obj::setTextColor(colorObj* tColor,colorObj* bColor) 	{ theOLED->setTextColor(tColor->getColor16(),bColor->getColor16()); }
void SSD_13XX_Obj::setTextSize(byte inSize)          								{ theOLED->setTextScale(inSize); }
void SSD_13XX_Obj::setTextWrap(boolean wrap)         								{ theOLED->setTextWrap(wrap); }
void SSD_13XX_Obj::setFont(const tFont *font) 		 									{ theOLED->setFont(font); }
void SSD_13XX_Obj::setCursor(int x,int y)        										{ theOLED->setCursor(gX(x),gY(y)); }
void SSD_13XX_Obj::drawText(char* inText)            								{ theOLED->print(inText); }
void SSD_13XX_Obj::fillScreen(colorObj* inColor)     								{ theOLED->fillScreen(inColor->getColor16()); }
void SSD_13XX_Obj::fillRect(int x,int y,word width,word height,colorObj* inColor) 									{ theOLED->fillRect(gX(x),gY(y),width,height,inColor->getColor16()); }
void SSD_13XX_Obj::drawRect(int x,int y,word width,word height,colorObj* inColor) 									{ theOLED->drawRect(gX(x),gY(y),width,height,inColor->getColor16()); }
void SSD_13XX_Obj::fillRoundRect(int x,int y,word width,word height,word radius,colorObj* inColor)	{ theOLED->fillRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void SSD_13XX_Obj::drawRoundRect(int x,int y,word width,word height,word radius,colorObj* inColor)	{ theOLED->drawRoundRect(gX(x),gY(y),width,height,radius,inColor->getColor16()); }
void SSD_13XX_Obj::drawCircle(int x,int y,word diam, colorObj* inColor) 														{ drawRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void SSD_13XX_Obj::fillCircle(int x,int y,word diam, colorObj* inColor) 														{ fillRoundRect(gX(x),gY(y),diam,diam,diam/2,inColor); }
void SSD_13XX_Obj::drawVLine(int x,int y,word height,colorObj* inColor) 														{ theOLED->drawFastVLine(gX(x),gY(y),height,inColor->getColor16()); }
void SSD_13XX_Obj::drawHLine(int x,int y,word width,colorObj* inColor) 															{ theOLED->drawFastHLine(gX(x),gY(y),width,inColor->getColor16()); }
void SSD_13XX_Obj::drawLine(int x,int y,int x2,int y2,colorObj* inColor) 														{ theOLED->drawLine(gX(x),gY(y),gX(x2),gY(y2),inColor->getColor16()); }
void SSD_13XX_Obj::drawPixel(int x,int y,colorObj* pColor) 																					{ theOLED->drawPixel(gX(x),gY(y),pColor->getColor16()); }


 
