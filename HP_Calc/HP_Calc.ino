#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <touchObj.h>
#include <drawObj.h>
#include <label.h>
#include <screenObj.h>


#include <SPI.h>
#include <Wire.h>

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include <calculator.h>
#include "calcButton.h"

#define  BACK_COLOR BLACK
#define  DISP_COLOR RED
#define  DISP_X     10
#define  DISP_Y     10
#define  TEXT_SIZE  2

calculator mCalc;
boolean    buttonPressed;
label XReg("0000000000000000");

//(char* inFStr,word locX, word locY,byte width, byte height);
calcButton btn1 = calcButton("1",30,100,50,20);
calcButton btn2 = calcButton("2",100,100,50,20);


void setup() {
  
  if (initScreen(INV_PORTRAIT)) {
    screen->fillScreen(BACK_COLOR);
  }
  XReg.setLocation(DISP_X,DISP_Y);
  XReg.setTextSize(TEXT_SIZE);
  XReg.setColors(DISP_COLOR,BACK_COLOR);
  XReg.setJustify(TEXT_RIGHT);
  XReg.setPrecision(mCalc.getFixVal());
  XReg.draw();
  
  btn1.drawSelf();
  buttonPressed = false;
}


void loop() {
    
  idle();
  if (buttonPressed) {
    XReg.setPrecision(mCalc.getFixVal());
    XReg.setValue(mCalc.getX());
    XReg.draw();
    buttonPressed = false;
  }
}

