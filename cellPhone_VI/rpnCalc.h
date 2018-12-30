#ifndef rpnCalc_h
#define rpnCalc_h

#include "panel.h"
#include <label.h>
#include <lineObj.h>
#include "calcButton.h"
#include "anime.h"
#include <calculator.h>
#include "litlOS.h" 


class rpnCalc : public panel {

  public:
          rpnCalc(blockFile* inFile,unsigned long rootID);
  virtual ~rpnCalc(void);

          void  loadScreen(void);
  virtual void  psetup(void);
  virtual void  ploop(void);

          label*          XReg;
          lineObj*        aLine;
          lineObj*        aLine2;
          lineObj*        aLine3;
          lineObj*        aLine4;
          degRadButton*   degRad;
          anime*          theSprit;
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

          closeButton*     goAway;
};


#endif
