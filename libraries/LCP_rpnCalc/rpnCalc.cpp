#include "rpnCalc.h"
#include <screen.h>

#include <calculator.h>
#include "calcButton.h"

#define  BACK_COLOR (&black)
#define  DISP_COLOR (&red)
#define  DISP_Y     28
#define  TEXT_SIZE  2

#define  LINE_X1     22
#define  LINE_X2     146
#define  LINE_Y      DISP_Y + 22

#define  LINE2_X1     LINE_X1
#define  LINE2_X2     LINE_X2
#define  LINE2_Y      LINE_Y + 2

#define  LINE3_X1     LINE_X1
#define  LINE3_X2     LINE_X2
#define  LINE3_Y      LINE2_Y + 2
            
#define DEG_RAD_X       156
#define DEG_RAD_Y       LINE_Y - 2
#define DEG_RAD_WIDTH   50
#define DEG_RAD_HEIGHT  15

#define BTN_WIDTH1  30
#define BTN_WIDTH2  50
#define BTN_WIDTH3  80

#define BTN_ROWA_1    64
#define BTN_ROWA_2    94
#define BTN_ROWA_3    124
#define BTN_ROWA_4    154

#define BTN_COLA_1    25
#define BTN_COLA_2    90
#define BTN_COLA_3    155

#define BTN_ROW_1    182
#define BTN_ROW_2    212
#define BTN_ROW_3    242
#define BTN_ROW_4    272
#define BTN_ROW_5    302

#define BTN_COL_1    25
#define BTN_COL_2    75
#define BTN_COL_3    125
#define BTN_COL_4    175


extern      bool            gSecondFx;
            calculator      mCalc;           // The calculator object. Feed it key strokes and it gives calculations.
            bool            buttonPressed;
				lilOS* calcOS = NULL;
          
rpnCalc::rpnCalc(lilOS* ourOS,int panelID)
	: panel(ourOS,panelID) {
	
	calcOS = ourOS;	// Set up our global OS pointer (For the buttons)
	degRad = new degRadButton(DEG_RAD_X,DEG_RAD_Y,DEG_RAD_WIDTH,DEG_RAD_HEIGHT);
		
	XReg = new label(BTN_COL_1, DISP_Y, (BTN_COL_4 + BTN_WIDTH1) - BTN_COL_1, 18, "0");

	aLine = new lineObj(LINE_X1,LINE_Y,LINE_X2,LINE_Y,DISP_COLOR);
	aLine2 = new lineObj(LINE2_X1,LINE2_Y,LINE2_X2,LINE2_Y,DISP_COLOR);  
	aLine3 = new lineObj(LINE3_X1,LINE3_Y,LINE3_X2,LINE3_Y,DISP_COLOR);  
 
	btn1 = new calcButton("1", BTN_COL_2, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN);
	btn2 = new calcButton("2", BTN_COL_3, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN);
	btn3 = new calcButton("3", BTN_COL_4, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN);
	btn4 = new calcButton("4", BTN_COL_2, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN);
	btn5 = new calcButton("5", BTN_COL_3, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN);
	btn6 = new calcButton("6", BTN_COL_4, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN);
	btn7 = new calcButton("7", BTN_COL_2, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN);
	btn8 = new calcButton("8", BTN_COL_3, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN);
	btn9 = new calcButton("9", BTN_COL_4, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN);
	btn0 = new calcButton("0", BTN_COL_2, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN);

	btnPlus = new calcButton("+", BTN_COL_1, BTN_ROW_3, BTN_WIDTH1, EDIT_BTN);
	btnMinus = new calcButton("-", BTN_COL_1, BTN_ROW_2, BTN_WIDTH1, EDIT_BTN);
	btnTimes = new calcButton("x", BTN_COL_1, BTN_ROW_4, BTN_WIDTH1, EDIT_BTN);
	btnDivide = new calcButton("/", BTN_COL_1, BTN_ROW_5, BTN_WIDTH1, EDIT_BTN);

	btnEnter = new calcButton("Enter", BTN_COL_1, BTN_ROW_1, BTN_WIDTH3, EDIT_BTN);

	btnChSgn = new calcButton("-x", BTN_COL_3, BTN_ROW_1, BTN_WIDTH1, EDIT_BTN);
	btnClrX = new calcButton("Cx", BTN_COL_4, BTN_ROW_1, BTN_WIDTH1, EDIT_BTN);
	btnPt = new calcButton(".", BTN_COL_3, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN);
	btnPi = new calcButton("Pi", BTN_COL_4, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN);

	btnInvrse = new calcButton("1/x", BTN_COLA_1, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);
	btnRoot = new calcButton("Srt","y^x", BTN_COLA_2, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);
	btnFix = new calcButton("Fix", BTN_COLA_3, BTN_ROWA_4, BTN_WIDTH2, FX_BTN);


	btnSwapXY = new calcButton("y~x","Rdn", BTN_COLA_1, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);
	btnSto = new calcButton(">p",">r", BTN_COLA_2, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);
	btnRcl = new calcButton("Sto","Rcl", BTN_COLA_3, BTN_ROWA_3, BTN_WIDTH2, FX_BTN);


	btnSin = new calcButton("sin","asn",BTN_COLA_1, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);
	btnCos = new calcButton("cos","acs",BTN_COLA_2, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);
	btnTan = new calcButton("tan","atn",BTN_COLA_3, BTN_ROWA_2, BTN_WIDTH2, FX_BTN);

	btnASin = new calcButton("Ln","e^x",BTN_COLA_1, BTN_ROWA_1, BTN_WIDTH2, FX_BTN);
	btnACos = new calcButton("log","t^x",BTN_COLA_2, BTN_ROWA_1, BTN_WIDTH2, FX_BTN);
	btnATan = new secondfxButton("2f",BTN_COLA_3, BTN_ROWA_1, BTN_WIDTH2, EDIT_BTN);

	gSecondFx = false;
}


rpnCalc::~rpnCalc(void) {

  if(degRad) delete degRad;
            
  if(XReg) delete XReg;
  
  if(aLine) delete aLine;
  if(aLine2) delete aLine2;
  
  if(btn1) delete btn1;
  if(btn2) delete btn2;
 if( btn3) delete btn3;
 if( btn4) delete btn4;
  if(btn5) delete btn5;
  if(btn6) delete btn6;
  if(btn7) delete btn7;
  if(btn8) delete btn8;
  if(btn9) delete btn9;
  if(btn0) delete btn0;
  
  if(btnPlus) delete btnPlus;
  if(btnMinus) delete btnMinus;
  if(btnTimes) delete btnTimes;
  if(btnDivide) delete btnDivide;
  
  if(btnEnter) delete btnEnter;
  
  if(btnChSgn) delete btnChSgn;
  if(btnClrX) delete btnClrX;
  if(btnPt) delete btnPt;
  if(btnPi) delete btnPi;
  
  if(btnInvrse) delete btnInvrse;
  if(btnRoot) delete btnRoot;
  if(btnFix) delete btnFix;
  
  
  if(btnSwapXY) delete btnSwapXY;
  if(btnSto) delete btnSto;
  if(btnRcl) delete btnRcl;
  
  
  if(btnSin) delete btnSin;
  if(btnCos) delete btnCos;
  if(btnTan) delete btnTan;
  
  if(btnASin) delete btnASin;
  if(btnACos) delete btnACos;
 if(btnATan) delete btnATan;
}


void  rpnCalc::setup(void) {

  screen->fillScreen(&black);
  loadScreen();
  buttonPressed = false;
}


void rpnCalc::loadScreen(void) {

  setupButtonColors();
  XReg->setTextSize(TEXT_SIZE);
  XReg->setColors(DISP_COLOR, BACK_COLOR);
  XReg->setJustify(TEXT_RIGHT);
  XReg->setPrecision(mCalc.getFixVal());
  addObj(XReg);

  aLine->setColor(DISP_COLOR);
  addObj(aLine);
  aLine2->setColor(DISP_COLOR);
  addObj(aLine2);
  aLine3->setColor(DISP_COLOR);
  addObj(aLine3);
  
  degRad->setColors(DISP_COLOR, &white, BACK_COLOR);
  addObj(degRad);

  addObj(btn1);
  addObj(btn2);
  addObj(btn3);
  addObj(btn4);
  addObj(btn5);
  addObj(btn6);
  addObj(btn7);
  addObj(btn8);
  addObj(btn9);
  addObj(btn0);

  addObj(btnPlus);
  addObj(btnMinus);
  addObj(btnTimes);
  addObj(btnDivide);

  addObj(btnEnter);

  addObj(btnChSgn);
  addObj(btnClrX);
  addObj(btnPt);
  addObj(btnPi);

  addObj(btnInvrse);
  addObj(btnRoot);
  addObj(btnFix);

  addObj(btnSin);
  addObj(btnCos);
  addObj(btnTan);
  
  addObj(btnSwapXY);
  addObj(btnSto);
  addObj(btnRcl);
  
  addObj(btnASin);
  addObj(btnACos);
  addObj(btnATan);
  
  screen->drawRect(BTN_COL_1-3,DISP_Y-3,((BTN_COL_4+BTN_WIDTH1)-BTN_COL_1)+5,22,DISP_COLOR);
 }


void  rpnCalc::loop(void) {

  if (buttonPressed) {
    XReg->setPrecision(mCalc.getFixVal());
    XReg->setValue(mCalc.getX());
    buttonPressed = false;
  }
}
