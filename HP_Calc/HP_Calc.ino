#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>
#include <adafruit_1947_Obj.h>

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_FT6206.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include <calculator.h>
#include "anime.h"
#include "calcButton.h"

#define  BACK_COLOR (&black)
#define  DISP_COLOR (&red)
#define  DISP_Y     4
#define  TEXT_SIZE  2

#define  LINE_X1     22
#define  LINE_X2     207
#define  LINE_Y      DISP_Y + 21

#define  LINE2_X1     125
#define  LINE2_X2     207
#define  LINE2_Y      DISP_Y + 24

#define  LINE3_X1     156
#define  LINE3_X2     207
#define  LINE3_Y      DISP_Y + 27
            
#define DEG_RAD_X       LINE_X1
#define DEG_RAD_Y       LINE_Y + 6
#define DEG_RAD_WIDTH   50
#define DEG_RAD_HEIGHT  15

#define SPRIT_X         DEG_RAD_X + 134
#define SPRIT_Y         LINE3_Y + 3
#define SPRIT_WIDTH     51
#define SPRIT_HEIGHT    8
#define SPRIT_TIME      25

#define  LINE4_Y        SPRIT_Y + SPRIT_HEIGHT + 2

#define BTN_WIDTH1  30
#define BTN_WIDTH2  50
#define BTN_WIDTH3  80

#define BTN_ROWA_1    60
#define BTN_ROWA_2    90
#define BTN_ROWA_3    120
#define BTN_ROWA_4    150

#define BTN_COLA_1    25
#define BTN_COLA_2    90
#define BTN_COLA_3    155

#define BTN_ROW_1    180
#define BTN_ROW_2    210
#define BTN_ROW_3    240
#define BTN_ROW_4    270
#define BTN_ROW_5    300

#define BTN_COL_1    25
#define BTN_COL_2    75
#define BTN_COL_3    125
#define BTN_COL_4    175

label XReg(BTN_COL_1, DISP_Y, (BTN_COL_4 + BTN_WIDTH1) - BTN_COL_1, 18, "0"); // ƒ
lineObj  aLine(LINE_X1,LINE_Y,LINE_X2,LINE_Y,DISP_COLOR);
lineObj  aLine2(LINE2_X1,LINE2_Y,LINE2_X2,LINE2_Y,DISP_COLOR);
lineObj  aLine3(LINE3_X1,LINE3_Y,LINE3_X2,LINE3_Y,DISP_COLOR);
lineObj  aLine4(LINE3_X1,LINE4_Y,LINE3_X2,LINE4_Y,DISP_COLOR);

degRadButton degRad(DEG_RAD_X,DEG_RAD_Y,DEG_RAD_WIDTH,DEG_RAD_HEIGHT);

anime theSprit(SPRIT_X,SPRIT_Y,SPRIT_WIDTH,SPRIT_HEIGHT,SPRIT_TIME);

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
calcButton btnRoot = calcButton("Srt","y^x", BTN_COLA_2, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);
calcButton btnFix = calcButton("Fix", BTN_COLA_3, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);


calcButton btnSwapXY = calcButton("y~x","Rdn", BTN_COLA_1, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);
calcButton btnSto = calcButton(">p",">r", BTN_COLA_2, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);
calcButton btnRcl = calcButton("Sto","Rcl", BTN_COLA_3, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);


calcButton btnSin = calcButton("sin","asn",BTN_COLA_1, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);
calcButton btnCos = calcButton("cos","acs",BTN_COLA_2, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);
calcButton btnTan = calcButton("tan","atn",BTN_COLA_3, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);

calcButton btnASin = calcButton("Ln","e^x",BTN_COLA_1, BTN_ROWA_1, BTN_WIDTH2, FX_BTN);
calcButton btnACos = calcButton("log","t^x",BTN_COLA_2, BTN_ROWA_1, BTN_WIDTH2, FX_BTN);
secondfxButton btnATan = secondfxButton("2f",BTN_COLA_3, BTN_ROWA_1, BTN_WIDTH2, EDIT_BTN);


calculator mCalc;           // The calculator object. Feed it key strokes and it gives calculations.
boolean    buttonPressed;

void setup() {
  //Serial.begin(9600);
  //while(!Serial);
  
  if (initScreen(ADAFRUIT_1947,PORTRAIT)) {
    //Serial.println(F("Got screen"));
    screen->fillScreen(&black);
    loadScreen();
    //Serial.println(F("Show Display"));
    screen->drawRect(BTN_COL_1 - 3, DISP_Y - 3, ((BTN_COL_4 + BTN_WIDTH1) - BTN_COL_1) + 5, 22, DISP_COLOR);
  } else Serial.println(F("Screen failed"));
  buttonPressed = false;
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, HIGH); //Means off.
}


void loadScreen(void) {

  //Serial.println(F("Loading screen"));
  setupButtonColors();
  XReg.setTextSize(TEXT_SIZE);
  XReg.setColors(DISP_COLOR, BACK_COLOR);
  XReg.setJustify(TEXT_RIGHT);
  XReg.setPrecision(mCalc.getFixVal());
  viewList.addObj(&XReg);

  aLine.setColor(DISP_COLOR);
  viewList.addObj(&aLine);
  aLine2.setColor(DISP_COLOR);
  viewList.addObj(&aLine2);
  aLine3.setColor(DISP_COLOR);
  viewList.addObj(&aLine3);
  aLine4.setColor(DISP_COLOR);
  viewList.addObj(&aLine4);
  
  degRad.setColors(DISP_COLOR, &white, BACK_COLOR);
  viewList.addObj(&degRad);

  theSprit.begin();
  viewList.addObj(&theSprit);
  
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

