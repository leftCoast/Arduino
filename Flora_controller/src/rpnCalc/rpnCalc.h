#ifndef rpnCalc_h
#define rpnCalc_h

#include <label.h>
#include <lineObj.h>
#include "calcButton.h"
#include <calculator.h>
#include <panel.h>
#include <litlOS.h>


// **********************************************************************
// 						THIS HAS TO POINT TO YOUR OS
// 		Your OS has to have calcApp defined in its apps enum.
// **********************************************************************
#include	"../../floraOS.h"
// **********************************************************************
// **********************************************************************
//  								Then this line :
//					"case calcApp : return new rpnCalc();"
// Goes into your "createPanel(int panelID)" method to load this panel.
// **********************************************************************



class rpnCalc : public panel {

  public:
          rpnCalc(void);
  virtual ~rpnCalc(void);

          void  loadScreen(void);
  virtual void  setup(void);
  virtual void  loop(void);

          label*          XReg;
          lineObj*        aLine;
          lineObj*        aLine2;
          lineObj*        aLine3;
          degRadButton*   degRad;
          calcButton*     btn1;
          calcButton*     btn2;
          calcButton*     btn3;
          calcButton*     btn4;
          calcButton*     btn5;
          calcButton*     btn6;
          calcButton*     btn7;
          calcButton*     btn8;
          calcButton*     btn9;
          calcButton*     btn0; 
          calcButton*     btnPlus;
          calcButton*     btnMinus;
          calcButton*     btnTimes;
          calcButton*     btnDivide;
          calcButton*     btnEnter;
          calcButton*     btnChSgn;
          calcButton*     btnClrX;
          calcButton*     btnPt;
          calcButton*     btnPi;
          calcButton*     btnInvrse;
          calcButton*     btnRoot;
          calcButton*     btnFix;
          calcButton*     btnSwapXY;
          calcButton*     btnSto;
          calcButton*     btnRcl;
          calcButton*     btnSin;
          calcButton*     btnCos;
          calcButton*     btnTan;    
          calcButton*     btnASin;
          calcButton*     btnACos;
          secondfxButton* btnATan;
};


#endif
