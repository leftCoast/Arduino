#include "displayObj.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>


// This set is for Arduino UNO & Teensy 3.2
#define ADAFRUIT_1431_SPI_CLK 		13
#define ADAFRUIT_1431_SPI_MOSI		11
#define ADAFRUIT_1431_SPI_MISO		12
#define ADAFRUIT_1431_SPI_OLED_CS	9
#define ADAFRUIT_1431_SPI_SD_CS	  4
#define ADAFRUIT_1431_SPI_DC			7
#define ADAFRUIT_1431_SPI_RST			8


/*
// This set is for Adafruit Feather MO #2995
#define ADAFRUIT_1431_SPI_CLK 	24
#define ADAFRUIT_1431_SPI_MOSI	23
#define ADAFRUIT_1431_SPI_MISO	22
#define ADAFRUIT_1431_SPI_CS	?   // Free to choose..
#define ADAFRUIT_1431_SPI_DC	?   // Free to choose..
#define ADAFRUIT_1431_SPI_RST	?   // Free to choose..
*/

// Rotation
#define PORTRAIT      0  // Wires up
#define LANDSCAPE     1  // Wires left
#define INV_PORTRAIT  2  // Wires down
#define INV_LANDSCAPE 3  // Wires right

class adafruit_1431_Obj : public displayObj {

  public :
  adafruit_1431_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST);
  ~adafruit_1431_Obj(void);
  
  virtual boolean dispObjBegin(void);
  virtual void    setRotation(byte inRotation);
  virtual void    setTextColor(colorObj* tColor);
  virtual void    setTextColor(colorObj* tColor,colorObj* bColor);
  virtual void    setTextSize(byte inSize);
  virtual void		setTextWrap(boolean wrap);
  virtual void 		setFont(const GFXfont* font);
  virtual void    setCursor(int inX,int inY);
  virtual void    drawText(char* inText);
  virtual void    fillScreen(colorObj* inColor);
  virtual void    fillRect(int locX,int locY,word width,word height,colorObj* inColor);
  virtual void    drawRect(int locX,int locY,word width,word height,colorObj* inColor);
  virtual void    fillRoundRect(int locX,int locY,word width,word height,word radius,colorObj* inColor);
  virtual void    drawRoundRect(int locX,int locY,word width,word height,word radius,colorObj* inColor);
  virtual void    drawCircle(int locX,int locY,word inDiam, colorObj* inColor);
  virtual void    fillCircle(int locX,int locY,word inDiam, colorObj* inColor);
  virtual void    drawVLine(int locX,int locY,word height,colorObj* inColor);
  virtual void    drawHLine(int locX,int locY,word width,colorObj* inColor);
  virtual void    drawLine(int locX,int locY,int locX2,int locY2,colorObj* inColor);
  virtual void    drawPixel(int locX,int locY,colorObj* pColor);
  
  private:
          Adafruit_SSD1351* theOLED;
          byte              cs;
          byte              dc;
          byte              mosi;
          byte              sclk;
          byte              rst;
};
