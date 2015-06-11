#ifndef screenObj_h
#define screenObj_h

#include <Adafruit_GFX.h>       // Core graphics library
#include <SPI.h>                // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>               // this is needed for FT6206
#include <Adafruit_FT6206.h>

#include <mapper.h>
#include <colorObj.h>

// Rotation
#define PORTRAIT      0  // USB up
#define LANDSCAPE     1  // USB left
#define INV_PORTRAIT  2  // USB down
#define INV_LANDSCAPE 3  // USB right


// The FT6206 uses hardware I2C (SCL/SDA)
// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9

// ****** screenObj ******

class screenObj : 
public Adafruit_ILI9341 {

public:
  screenObj(void);

  boolean init(int inRotation=PORTRAIT);
  
  void    fillRectGradient(word inX,word inY,word inXSize,word inYSize,colorObj* startColor,colorObj* endColor,boolean vertical=true);
  void    fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical=true);
  //void    drawPixelInvert(word x,word y);
  //void    frameRectInvert(TS_Point loc, word width,word height);
  TS_Point  getPoint(void);
  boolean   touched(TS_Point inPt);
  

private :
  Adafruit_FT6206* ts;
};

extern screenObj* screen;

boolean initScreen(int inRotation);


#endif





