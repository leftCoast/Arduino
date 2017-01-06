#include "displayObj.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

class adafruit_1431_Obj : public displayObj {

  public :
  adafruit_1431_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST);
  ~adafruit_1431_Obj(void);
  
  virtual boolean dispObjBegin(void);
  virtual void    setTextColor(colorObj* tColor);
  virtual void    setTextColor(colorObj* tColor,colorObj* bColor);
  virtual void    setTextSize(byte inSize);
  virtual void    setCursor(int inX,int inY);
  virtual void    drawText(char* inText);
  virtual void    fillScreen(colorObj* inColor);
  virtual void    fillRect(word locX,word locY,word width,word height,colorObj* inColor);
  virtual void    drawRect(word locX,word locY,word width,word height,colorObj* inColor);
  virtual void    drawPixel(word x,word y,colorObj* pColor);
  
  private:
          Adafruit_SSD1351* theOLED;
          byte              cs;
          byte              dc;
          byte              mosi;
          byte              sclk;
          byte              rst;
};