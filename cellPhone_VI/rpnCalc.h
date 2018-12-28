#ifndef rpnCalc_h
#define rpnCalc_h


class rpnCalc : public panel {

  public:
          rpnCalc(blockFile* inFile,unsigned long rootID);
  virtual ~rpnCalc(void);

          void  loadScreen(void);
  virtual void  panelSetup(void);
  virtual void  panelLoop(void);

          label*           XReg(BTN_COL_1, DISP_Y, (BTN_COL_4 + BTN_WIDTH1) - BTN_COL_1, 18, "0"); 
          lineObj*         aLine(LINE_X1,LINE_Y,LINE_X2,LINE_Y,DISP_COLOR);
          lineObj*         aLine2(LINE2_X1,LINE2_Y,LINE2_X2,LINE2_Y,DISP_COLOR);
          lineObj*         aLine3(LINE3_X1,LINE3_Y,LINE3_X2,LINE3_Y,DISP_COLOR);
          lineObj*         aLine4(LINE3_X1,LINE4_Y,LINE3_X2,LINE4_Y,DISP_COLOR);
          
          degRadButton*    degRad(DEG_RAD_X,DEG_RAD_Y,DEG_RAD_WIDTH,DEG_RAD_HEIGHT);
          
          anime*           theSprit(SPRIT_X,SPRIT_Y,SPRIT_WIDTH,SPRIT_HEIGHT,SPRIT_TIME);
          
          calcButton      btn1;
          calcButton      btn2;
          calcButton      btn3;
          calcButton      btn4;
          calcButton      btn5;
          calcButton      btn6;
          calcButton      btn7;
          calcButton      btn8;
          calcButton      btn9;
          calcButton      btn0;
          
          calcButton      btnPlus;
          calcButton      btnMinus;
          calcButton      btnTimes;
          calcButton      btnDivide;
          
          calcButton      btnEnter;
          
          calcButton      btnChSgn;
          calcButton      btnClrX;
          calcButton      btnPt;
          calcButton      btnPi;
          
          calcButton      btnInvrse;
          calcButton      btnRoot;
          calcButton      btnFix;
          
          
          calcButton      btnSwapXY;
          calcButton      btnSto;
          calcButton      btnRcl;
          
          
          calcButton      btnSin;
          calcButton      btnCos;
          calcButton      btnTan;
          
          calcButton      btnASin;
          calcButton      btnACos;
          secondfxButton  btnATan;
          
          
          calculator      mCalc;           // The calculator object. Feed it key strokes and it gives calculations.
          boolean         buttonPressed;
          boolean         gSecondFx;
};






#endif
