#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <label.h>
#include <screenObj.h>


#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include <calculator.h>

#define  BACK_COLOR BLACK
#define  DISP_COLOR RED
#define  DISP_X     10
#define  DISP_Y     10
#define  TEXT_SIZE  2

calculator mCalc;
label XReg("0000000000000000");

Adafruit_GFX_Button btn1;

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
  
  btn1.initButton((Adafruit_GFX*)screen, 30, 100, 
		      50, 20, 
		      WHITE, WHITE,BLACK,
		      "1",2);
  btn1.drawButton();
  mCalc.buttonClick("1");
  mCalc.buttonClick("4");
  mCalc.buttonClick("Enter");
  mCalc.buttonClick("3");
  mCalc.buttonClick("x");
}

boolean pressButton() {
  if (btn1.justReleased()) {
    mCalc.buttonClick("1");
    return true;
  }
  return false;
}


void loop() {
  if (pressButton) {
    XReg.setPrecision(mCalc.getFixVal());
    XReg.setValue(mCalc.getX());
    XReg.draw();
  }
}

