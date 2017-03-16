#include "displayObj.h"
#include "SSD_13XX.h"
#include <LC_SPI.h>


// Teensy 3.x can use: 2,6,10,15,20,21,22,23
// Arduino's 8 bit: any pins

// Rotation
#define PORTRAIT      0  // Wires up
#define LANDSCAPE     1  // Wires left
#define INV_PORTRAIT  2  // Wires down
#define INV_LANDSCAPE 3  // Wires right

class SSD_13XX_Obj : public displayObj {

  public :
  SSD_13XX_Obj(byte inCS,byte inRST);	// OLED MUST have reset. Won't work otherwise.
  ~SSD_13XX_Obj(void);
  
  virtual boolean dispObjBegin(void);
  virtual void    setRotation(byte inRotation);
  virtual void    setTextColor(colorObj* tColor);
  virtual void    setTextColor(colorObj* tColor,colorObj* bColor);
  virtual void    setTextSize(byte inSize);
  virtual void		setTextWrap(boolean wrap);
  virtual void 		setFont(const tFont *font);
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
          SSD_13XX* theOLED;
          byte      cs;
          byte      dc;
          byte      mosi;
          byte      sclk;
          byte      rst;
};
