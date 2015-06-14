#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

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

#define BTN_WIDTH1  40
#define BTN_WIDTH2  100
#define BTN_SPACE   10
//(char* inFStr,word locX, word locY,byte width);

calcButton btn1 = calcButton("1",30,100,BTN_WIDTH1);
calcButton btn2 = calcButton("2",30+BTN_WIDTH1+BTN_SPACE,100,BTN_WIDTH1);
calcButton btnEnter = calcButton("Enter",30,100-18-BTN_SPACE,BTN_WIDTH2);


void setup() {
  //Serial.begin(9600);
  if (initScreen(INV_PORTRAIT)) {
    screen->fillScreen(BACK_COLOR);
  }
  XReg.setLocation(DISP_X,DISP_Y);
  XReg.setTextSize(TEXT_SIZE);
  XReg.setColors(DISP_COLOR,BACK_COLOR);
  XReg.setJustify(TEXT_RIGHT);
  XReg.setPrecision(mCalc.getFixVal());
  viewList.addObj(&XReg);
  viewList.addObj(&btn1);
  //viewList.addObj(&btn2);
  //viewList.addObj(&btnEnter);
  
  buttonPressed = false;
}


void loop() {
    
  idle();
  if (buttonPressed) {
    XReg.setPrecision(mCalc.getFixVal());
    XReg.setValue(mCalc.getX());
    buttonPressed = false;
  }
}

