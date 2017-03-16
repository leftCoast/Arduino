#include "displayObj.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <LC_SPI.h>


// Rotation
#define PORTRAIT      0  // Wires up
#define LANDSCAPE     1  // Wires left
#define INV_PORTRAIT  2  // Wires down
#define INV_LANDSCAPE 3  // Wires right

class adafruit_1431_Obj : public displayObj {

  public :
  adafruit_1431_Obj(byte inCS,byte inRST=-1);
  ~adafruit_1431_Obj(void);
  
  virtual boolean dispObjBegin(void);
  virtual void    setRotation(byte inRotation);
  virtual void    setTextColor(colorObj* tColor);
  virtual void    setTextColor(colorObj* tColor,colorObj* bColor);
  virtual void    setTextSize(byte inSize);
  virtual void		setTextWrap(boolean wrap);
  virtual void 		setFont(const GFXfont* font);
  virtual void    setCursor(int x,int y);
  virtual void    drawText(char* inText);
  virtual void    fillScreen(colorObj* inColor);
  virtual void    fillRect(int x,int y,word width,word height,colorObj* inColor);
  virtual void    drawRect(int x,int y,word width,word height,colorObj* inColor);
  virtual void    fillRoundRect(int x,int y,word width,word height,word radius,colorObj* inColor);
  virtual void    drawRoundRect(int x,int y,word width,word height,word radius,colorObj* inColor);
  virtual void    drawCircle(int x,int y,word diam, colorObj* inColor);
  virtual void    fillCircle(int x,int y,word diam, colorObj* inColor);
  virtual void    drawVLine(int x,int y,word height,colorObj* inColor);
  virtual void    drawHLine(int x,int y,word width,colorObj* inColor);
  virtual void    drawLine(int x,int y,int x2,int y2,colorObj* inColor);
  virtual void    drawPixel(int x,int y,colorObj* pColor);
  
  private:
          Adafruit_SSD1351* theOLED;
          byte              cs;
          byte              rst;
};
