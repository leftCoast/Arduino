#include "displayObj.h"
#include "SSD_13XX.h"


// This set is for Arduino UNO & Teensy 3.2
// There are only a few ways to do this.
// So just standardise it this way
#define SSD_13XX_SPI_CLK 	  13
#define SSD_13XX_SPI_MISO	  12
#define SSD_13XX_SPI_MOSI	  11
#define SSD_13XX_SPI_OLED_CS  10
#define SSD_13XX_SPI_DC		  9
#define SSD_13XX_SPI_RST	  6bb
#define SSD_13XX_SPI_SD_CS    20

// Teensy 3.x can use: 2,6,10,15,20,21,22,23
// Arduino's 8 bit: any

// Rotation
#define PORTRAIT      0  // Wires up
#define LANDSCAPE     1  // Wires left
#define INV_PORTRAIT  2  // Wires down
#define INV_LANDSCAPE 3  // Wires right

class SSD_13XX_Obj : public displayObj {

  public :
  SSD_13XX_Obj(byte inCS,byte inDC,byte inMOSI,byte inSCLK,byte inRST);
  ~SSD_13XX_Obj(void);
  
  virtual boolean dispObjBegin(void);
  virtual void    setRotation(byte inRotation);
  virtual void    setTextColor(colorObj* tColor);
  virtual void    setTextColor(colorObj* tColor,colorObj* bColor);
  virtual void    setTextSize(byte inSize);
  virtual void		setTextWrap(boolean wrap);
  virtual void 		setFont(const tFont *font);
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
          SSD_13XX* theOLED;
          byte      cs;
          byte      dc;
          byte      mosi;
          byte      sclk;
          byte      rst;
};
