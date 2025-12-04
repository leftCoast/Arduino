
#include <lineObj.h>
#include <rpnCalc.h>
#include <calculator.h>
#include <calcButton.h>

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


          
rpnCalc::rpnCalc(int panelID)
	: panel(panelID) {
	
	secondFx		= false;
	btnPressed	= false;
}


// Nothing to do. The display will delete all the allocated items.
rpnCalc::~rpnCalc(void) { }


void  rpnCalc::setup(void) {

	screen->fillScreen(&black);
	setupButtonColors();
	
	XReg = new label(BTN_COL_1, DISP_Y, (BTN_COL_4 + BTN_WIDTH1) - BTN_COL_1, 18, "0");
	if (XReg) {
  		XReg->setTextSize(TEXT_SIZE);
  		XReg->setColors(DISP_COLOR, BACK_COLOR);
  		XReg->setJustify(TEXT_RIGHT);
  		XReg->setPrecision(calc.getFixVal());
  		addObj(XReg);
  	}
	lineObj* aLine = new lineObj(LINE_X1,LINE_Y,LINE_X2,LINE_Y,DISP_COLOR);
	if (aLine) {
  		aLine->setColor(DISP_COLOR);
  		addObj(aLine);
  	}
  	lineObj* aLine2 = new lineObj(LINE2_X1,LINE2_Y,LINE2_X2,LINE2_Y,DISP_COLOR);
  	if (aLine2) {
  		aLine2->setColor(DISP_COLOR);
  		addObj(aLine2);
  	}
  	lineObj* aLine3 = new lineObj(LINE3_X1,LINE3_Y,LINE3_X2,LINE3_Y,DISP_COLOR);
  	if (aLine3) {
  		aLine3->setColor(DISP_COLOR);
  		addObj(aLine3);
  	}
  	degRadButton* degRad = new degRadButton(DEG_RAD_X,DEG_RAD_Y,DEG_RAD_WIDTH,DEG_RAD_HEIGHT,this);
  	if (degRad) {
  		degRad->setColors(DISP_COLOR, &white, BACK_COLOR);
  		addObj(degRad);
  	}
	calcButton* btn1 = new calcButton("1", BTN_COL_2, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn1) addObj(btn1);
	calcButton* btn2 = new calcButton("2", BTN_COL_3, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn2) addObj(btn2);
	calcButton* btn3 = new calcButton("3", BTN_COL_4, BTN_ROW_4, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn3) addObj(btn3);
	calcButton* btn4 = new calcButton("4", BTN_COL_2, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn4) addObj(btn4);
	calcButton* btn5 = new calcButton("5", BTN_COL_3, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn5) addObj(btn5);
	calcButton* btn6 = new calcButton("6", BTN_COL_4, BTN_ROW_3, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn6) addObj(btn6);
	calcButton* btn7 = new calcButton("7", BTN_COL_2, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn7) addObj(btn7);
	calcButton* btn8 = new calcButton("8", BTN_COL_3, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn8) addObj(btn8);
	calcButton* btn9 = new calcButton("9", BTN_COL_4, BTN_ROW_2, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn9) addObj(btn9);
	calcButton* btn0 = new calcButton("0", BTN_COL_2, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN,this);
	if (btn0) addObj(btn0);
	
	calcButton* btnPlus = new calcButton("+", BTN_COL_1, BTN_ROW_3, BTN_WIDTH1, EDIT_BTN,this);
	if (btnPlus) addObj(btnPlus);
	calcButton* btnMinus = new calcButton("-", BTN_COL_1, BTN_ROW_2, BTN_WIDTH1, EDIT_BTN,this);
	if (btnMinus) addObj(btnMinus);
	calcButton* btnTimes = new calcButton("x", BTN_COL_1, BTN_ROW_4, BTN_WIDTH1, EDIT_BTN,this);
	if (btnTimes) addObj(btnTimes);
	calcButton* btnDivide = new calcButton("/", BTN_COL_1, BTN_ROW_5, BTN_WIDTH1, EDIT_BTN,this);
	if (btnDivide) addObj(btnDivide);
	
	calcButton* btnEnter = new calcButton("Enter", BTN_COL_1, BTN_ROW_1, BTN_WIDTH3, EDIT_BTN,this);
	if (btnEnter) addObj(btnEnter);
	
	calcButton* btnChSgn = new calcButton("-x", BTN_COL_3, BTN_ROW_1, BTN_WIDTH1, EDIT_BTN,this);
	if (btnChSgn) addObj(btnChSgn);
	calcButton* btnClrX = new calcButton("Cx", BTN_COL_4, BTN_ROW_1, BTN_WIDTH1, EDIT_BTN,this);
	if (btnClrX) addObj(btnClrX);
	calcButton* btnPt = new calcButton(".", BTN_COL_3, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN,this);
	if (btnPt) addObj(btnPt);
	calcButton* btnPi = new calcButton("Pi", BTN_COL_4, BTN_ROW_5, BTN_WIDTH1, NUMBER_BTN,this);
	if (btnPi) addObj(btnPi);

	calcButton* btnInvrse = new calcButton("1/x", BTN_COLA_1, BTN_ROWA_4, BTN_WIDTH2, FX_BTN,this);
	if (btnInvrse) addObj(btnInvrse);
	calcButton* btnRoot = new calcButton("Srt","y^x", BTN_COLA_2, BTN_ROWA_4, BTN_WIDTH2, FX_BTN,this);
	if (btnRoot) addObj(btnRoot);
	calcButton* btnFix = new calcButton("Fix", BTN_COLA_3, BTN_ROWA_4, BTN_WIDTH2, FX_BTN,this);
	if (btnFix) addObj(btnFix);

	calcButton* btnSin = new calcButton("sin","asn",BTN_COLA_1, BTN_ROWA_2, BTN_WIDTH2, FX_BTN,this);
	if (btnSin) addObj(btnSin);
	calcButton* btnCos = new calcButton("cos","acs",BTN_COLA_2, BTN_ROWA_2, BTN_WIDTH2, FX_BTN,this);
	if (btnCos) addObj(btnCos);
	calcButton* btnTan = new calcButton("tan","atn",BTN_COLA_3, BTN_ROWA_2, BTN_WIDTH2, FX_BTN,this);
	if (btnTan) addObj(btnTan);
  
	calcButton* btnSwapXY = new calcButton("y~x","Rdn", BTN_COLA_1, BTN_ROWA_3, BTN_WIDTH2, FX_BTN,this);
	if (btnSwapXY) addObj(btnSwapXY);
	calcButton* btnSto = new calcButton(">p",">r", BTN_COLA_2, BTN_ROWA_3, BTN_WIDTH2, FX_BTN,this);
	if (btnSto) addObj(btnSto);
	calcButton* btnRcl = new calcButton("Sto","Rcl", BTN_COLA_3, BTN_ROWA_3, BTN_WIDTH2, FX_BTN,this);
	if (btnRcl) addObj(btnRcl);
  
	calcButton* btnLogE = new calcButton("Ln","e^x",BTN_COLA_1, BTN_ROWA_1, BTN_WIDTH2, FX_BTN,this);
	if (btnLogE) addObj(btnLogE);
	calcButton* btnLogT = new calcButton("log","t^x",BTN_COLA_2, BTN_ROWA_1, BTN_WIDTH2, FX_BTN,this);
	if(btnLogT) addObj(btnLogT);
	secondfxButton* btn2ndF = new secondfxButton("2f",BTN_COLA_3, BTN_ROWA_1, BTN_WIDTH2, EDIT_BTN,this);
	if(btn2ndF) addObj(btn2ndF);
  
	screen->drawRect(BTN_COL_1-3,DISP_Y-3,((BTN_COL_4+BTN_WIDTH1)-BTN_COL_1)+5,22,DISP_COLOR);
	btnPressed = false;
 }
 
 
void rpnCalc::setupButtonColors(void) {

  numberActiveBColor.setColor(LC_WHITE);
  numberActiveTColor.setColor(LC_BLACK);
  numberClickedBColor.setColor(LC_BLACK);
  numberClickedTColor.setColor(LC_WHITE);
  
  editActiveBColor.setColor(LC_BLUE);
  editActiveTColor.setColor(LC_WHITE);
  editClickedBColor.setColor(LC_WHITE);
  editClickedTColor.setColor(LC_BLACK);
  
  fxActiveBColor.setColor(LC_BLACK);
  fxActiveTColor.setColor(LC_WHITE);
  fxClickedBColor.setColor(LC_WHITE);
  fxClickedTColor.setColor(LC_BLACK);

  numberActiveBColor.blend(&black, 40);
  numberActiveBColor.blend(&yellow, 25);
  numberActiveTColor.blend(&black, 60);
  editActiveBColor.blend(&white, 40);
  fxActiveBColor.blend(&white, 20);
}


void rpnCalc::buttonClick(const char* btnID) { calc.buttonClick(btnID); }


bool rpnCalc::getDegrees(void) { return calc.getDegrees(); }


void rpnCalc::toggleDegrees(void) { calc.toggleDegrees(); }


void rpnCalc::toggle2ndFx(void) { secondFx = !secondFx; }


void rpnCalc::loop(void) {

	if (btnPressed) {										// Got a click!
		if (XReg) {											// Sanity..
			XReg->setPrecision(calc.getFixVal());	// See if precision has changed..
			XReg->setValue(calc.getX());				// Grab new value and display.
		}
    	btnPressed = false;
	}
}
