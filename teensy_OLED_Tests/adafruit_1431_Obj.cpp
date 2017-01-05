#include <SPI.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1351.h> 
#include "adafruit_1431_Obj.h"


#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128
#define NUM_PIXELS    16384 // Yeah, but I don't want to calculate this. In a hurry!

adafruit_1431_Obj::adafruit_1431_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST)
  :displayObj(true,true,false,true) {

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

  
void adafruit_1431_Obj::begin(void) { 

  theOLED = new Adafruit_SSD1351(cs, dc, mosi, sclk, rst);
  if (theOLED!=NULL) {
    theOLED->begin();
  }
}


void adafruit_1431_Obj::setTextColor(colorObj* inColor)   { theOLED->setTextColor(inColor->getColor16()); }
void adafruit_1431_Obj::setTextSize(byte inSize)          { theOLED->setTextColor(inSize); }
void adafruit_1431_Obj::setCursor(int inX,int inY)        { theOLED->setCursor(inX,inY); }
void adafruit_1431_Obj::drawText(char* inText)            { theOLED->print(inText); }
void adafruit_1431_Obj::fillScreen(colorObj* inColor)     { theOLED->fillScreen(inColor->getColor16()); }


 
