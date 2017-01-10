#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <label.h>
#include <screen.h>
#include <adafruit_1947_Obj.h>

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include <calculator.h>
#include "calcButton.h"

#define  BACK_COLOR (&black)
#define  DISP_COLOR (&red)
#define  DISP_Y     10
#define  TEXT_SIZE  2

#define BTN_WIDTH1  30
#define BTN_WIDTH2  50
#define BTN_WIDTH3  80

#define BTN_ROWA_1    40
#define BTN_ROWA_2    70
#define BTN_ROWA_3    100
#define BTN_ROWA_4    130

#define BTN_COLA_1    25
#define BTN_COLA_2    90
#define BTN_COLA_3    155

#define BTN_ROW_1    160
#define BTN_ROW_2    190
#define BTN_ROW_3    220
#define BTN_ROW_4    250
#define BTN_ROW_5    280

#define BTN_COL_1    25
#define BTN_COL_2    75
#define BTN_COL_3    125
#define BTN_COL_4    175

label XReg(BTN_COL_1, DISP_Y, (BTN_COL_4 + BTN_WIDTH1) - BTN_COL_1, 18, "0");

calcButton btn1 = calcButton("1", BTN_COL_2, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN);
calcButton btn2 = calcButton("2", BTN_COL_3, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN);
calcButton btn3 = calcButton("3", BTN_COL_4, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN);
calcButton btn4 = calcButton("4", BTN_COL_2, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN);
calcButton btn5 = calcButton("5", BTN_COL_3, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN);
calcButton btn6 = calcButton("6", BTN_COL_4, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN);
calcButton btn7 = calcButton("7", BTN_COL_2, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN);
calcButton btn8 = calcButton("8", BTN_COL_3, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN);
calcButton btn9 = calcButton("9", BTN_COL_4, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN);
calcButton btn0 = calcButton("0", BTN_COL_2, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN);

calcButton btnPlus = calcButton("+", BTN_COL_1, BTN_ROW_3, BTN_WIDTH1, EDIT_BTN);
calcButton btnMinus = calcButton("-", BTN_COL_1, BTN_ROW_2, BTN_WIDTH1, EDIT_BTN);
calcButton btnTimes = calcButton("x", BTN_COL_1, BTN_ROW_4, BTN_WIDTH1, EDIT_BTN);
calcButton btnDivide = calcButton("/", BTN_COL_1, BTN_ROW_5, BTN_WIDTH1, EDIT_BTN);

calcButton btnEnter = calcButton("Enter", BTN_COL_1, BTN_ROW_1, BTN_WIDTH3, EDIT_BTN);

calcButton btnChSgn = calcButton("-x", BTN_COL_3, BTN_ROW_1, BTN_WIDTH1, EDIT_BTN);
calcButton btnClrX = calcButton("Cx", BTN_COL_4, BTN_ROW_1, BTN_WIDTH1, EDIT_BTN);
calcButton btnPt = calcButton(".", BTN_COL_3, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN);
calcButton btnPi = calcButton("Pi", BTN_COL_4, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN);

calcButton btnInvrse = calcButton("1/x", BTN_COLA_1, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);
calcButton btnRoot = calcButton("Srt", BTN_COLA_2, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);
//calcButton btnRoot = calcButton("√x", BTN_COLA_2, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);
calcButton btnFix = calcButton("Fix", BTN_COLA_3, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);


calcButton btnSwapXY = calcButton("x~y", BTN_COLA_1, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);
//calcButton btnRollDn = calcButton("Rdn", BTN_COL_3, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);
calcButton btnSto = calcButton("Sto", BTN_COLA_2, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);
calcButton btnRcl = calcButton("Rcl", BTN_COLA_3, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);


calcButton btnSin = calcButton("sin",BTN_COLA_1, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);
calcButton btnCos = calcButton("cos",BTN_COLA_2, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);
calcButton btnTan = calcButton("tan",BTN_COLA_3, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);

calcButton btnASin = calcButton("asn",BTN_COLA_1, BTN_ROWA_1, BTN_WIDTH2, FX_BTN);
calcButton btnACos = calcButton("acs",BTN_COLA_2, BTN_ROWA_1, BTN_WIDTH2, FX_BTN);
calcButton btnATan = calcButton("atn",BTN_COLA_3, BTN_ROWA_1, BTN_WIDTH2, FX_BTN);


calculator mCalc;           // The calculator object. Feed it key strokes and it gives calculations.
boolean    buttonPressed;

void setup() {
  //Serial.begin(9600);
  //while(!Serial);
  
  if (initScreen(ADAFRUIT_1947,PORTRAIT)) {
    Serial.println(F("Got screen"));
    screen->fillScreen(&black);
    loadScreen();
    Serial.println(F("Show Display"));
    screen->drawRect(BTN_COL_1 - 3, DISP_Y - 3, ((BTN_COL_4 + BTN_WIDTH1) - BTN_COL_1) + 5, 22, DISP_COLOR);
  } else Serial.println(F("Screen failed"));
  buttonPressed = false;
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, HIGH); //Means off.
}


void loadScreen(void) {

  Serial.println(F("Loading screen"));
  setupButtonColors();
  XReg.setTextSize(TEXT_SIZE);
  XReg.setColors(DISP_COLOR, BACK_COLOR);
  XReg.setJustify(TEXT_RIGHT);
  XReg.setPrecision(mCalc.getFixVal());
  viewList.addObj(&XReg);

  viewList.addObj(&btn1);
  viewList.addObj(&btn2);
  viewList.addObj(&btn3);
  viewList.addObj(&btn4);
  viewList.addObj(&btn5);
  viewList.addObj(&btn6);
  viewList.addObj(&btn7);
  viewList.addObj(&btn8);
  viewList.addObj(&btn9);
  viewList.addObj(&btn0);

  viewList.addObj(&btnPlus);
  viewList.addObj(&btnMinus);
  viewList.addObj(&btnTimes);
  viewList.addObj(&btnDivide);

  viewList.addObj(&btnEnter);

  viewList.addObj(&btnChSgn);
  viewList.addObj(&btnClrX);
  viewList.addObj(&btnPt);
  viewList.addObj(&btnPi);

  viewList.addObj(&btnInvrse);
  viewList.addObj(&btnRoot);
  viewList.addObj(&btnFix);

  viewList.addObj(&btnSin);
  viewList.addObj(&btnCos);
  viewList.addObj(&btnTan);
  
  viewList.addObj(&btnSwapXY);
  viewList.addObj(&btnSto);
  viewList.addObj(&btnRcl);
  
  viewList.addObj(&btnASin);
  viewList.addObj(&btnACos);
  viewList.addObj(&btnATan);
}


void loop() {

  idle();
  if (buttonPressed) {
    XReg.setPrecision(mCalc.getFixVal());
    XReg.setValue(mCalc.getX());
    buttonPressed = false;
  }
}

