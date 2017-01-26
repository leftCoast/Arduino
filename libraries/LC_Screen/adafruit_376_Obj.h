#ifndef adafruit_376_obj_h
#define adafruit_376_obj_h

#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#include <displayObj.h>

// Why does it care?
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif


#ifndef USE_ADAFRUIT_SHIELD_PINOUT 
 #error "This sketch is intended for use with the TFT LCD Shield. Make sure that USE_ADAFRUIT_SHIELD_PINOUT is #defined in the Adafruit_TFTLCD.h library file."
#endif

// These are the pins for the shield!
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin


#ifdef __SAM3X8E__
  #define TS_MINX 125
  #define TS_MINY 170
  #define TS_MAXX 880
  #define TS_MAXY 940
#else
  #define TS_MINX  150
  #define TS_MINY  120
  #define TS_MAXX  920
  #define TS_MAXY  940
#endif

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Rotation
#define PORTRAIT      0  // USB up
#define LANDSCAPE     1  // USB left
#define INV_PORTRAIT  2  // USB down
#define INV_LANDSCAPE 3  // USB right

#ifndef swap
#define swap(a, b) { int16_t t = a; a = b; b = t; }
#endif

class adafruit_376_Obj : public displayObj {

  public :
  adafruit_376_Obj(void);
  ~adafruit_376_Obj(void);
  
  virtual boolean dispObjBegin(void);
  virtual void    setRotation(byte inRotation);
  virtual void    setTextColor(colorObj* tColor);
  virtual void    setTextColor(colorObj* tColor,colorObj* bColor);
  virtual void    setTextSize(byte inSize);
  virtual void    setTextWrap(boolean wrap);
  virtual void    setCursor(word inX,word inY);
  virtual void    drawText(char* inText);
  virtual void    fillScreen(colorObj* inColor);
  virtual void    fillRect(word locX,word locY,word width,word height,colorObj* inColor);
  virtual void    drawRect(word locX,word locY,word width,word height,colorObj* inColor);
  virtual void    fillRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor);
  virtual void    drawRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor);
  virtual void    drawCircle(word locX,word locY,word inDiam, colorObj* inColor);
  virtual void    fillCircle(word locX,word locY,word inDiam, colorObj* inColor);
  virtual void    drawVLine(word locX,word locY,word height,colorObj* inColor);
  virtual void    drawHLine(word locX,word locY,word width,colorObj* inColor);
  virtual void    drawLine(word locX,word locY,word locX2,word locY2,colorObj* inColor);
  virtual void    drawPixel(word x,word y,colorObj* pColor);
  
  virtual point     getPoint(void);
  virtual boolean   touched(void);
  
  private:
          TouchScreen*		  cTS;
          Adafruit_TFTLCD*  theTFT;
};

#endif