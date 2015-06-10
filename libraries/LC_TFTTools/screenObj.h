#ifndef screenObj_h
#define screenObj_h

#include <Adafruit_GFX.h>       // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>

#include <mapper.h>
#include <colorObj.h>

// Rotation
#define PORTRAIT      0  // USB up
#define LANDSCAPE     1  // USB left
#define INV_PORTRAIT  2  // USB down
#define INV_LANDSCAPE 3  // USB right


// The FT6206 uses hardware I2C (SCL/SDA)
//Adafruit_FT6206 ctp = Adafruit_FT6206();

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);





// ****** screenObj ******

class screenObj : 
public Adafruit_ILI9341 {

public:
  screenObj(void);

  boolean init(int inRotation=PORTRAIT);
  
  void    fillRectGradient(word inX,word inY,word inXSize,word inYSize,word startColor16,word endColor16,boolean vertical=true);
  void    fillRectGradient(word inX,word inY,word inXSize,word inYSize,colorObj* startColor,colorObj* endColor,boolean vertical=true);
  void    fillScreenGradient(word startColor16,word endColor16,boolean vertical=true);
  void    fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical=true);
  void    drawPixelInvert(word x,word y);
  void    frameRectInvert(TSPoint loc, word width,word height);
  TSPoint   getPoint(void);
  boolean pressed(TSPoint inPt);
  

private :
  mapper*          xMapper;
  mapper*          yMapper;
  Adafruit_FT6206* ts;
};

extern screenObj* screen;

boolean initScreen(int inRotation);


#endif





