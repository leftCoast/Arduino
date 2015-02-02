#ifndef screenObj_h
#define screenObj_h

#include <Adafruit_GFX.h>       // Core graphics library
#include <Adafruit_TFTLCD.h>    // Hardware-specific library
#include <TouchScreen.h>

#include "mapper.h"
#include "colorObj.h"

#if not defined USE_ADAFRUIT_SHIELD_PINOUT 
#error "For use with the shield, make sure to #define USE_ADAFRUIT_SHIELD_PINOUT in the TFTLCD.h library file"
#endif

// These are the pins for the shield!
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate

#define OHMS   300
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0 

// Limits for detecting touches
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// Rotation
#define PORTRAIT      0  // USB up
#define LANDSCAPE     1  // USB left
#define INV_PORTRAIT  2  // USB down
#define INV_LANDSCAPE 3  // USB right


// ****** screenObj ******

class screenObj : 
public Adafruit_TFTLCD {

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
  mapper* xMapper;
  mapper* yMapper;
  TouchScreen* ts;
};

extern screenObj* screen;

boolean initScreen(int inRotation);


#endif





