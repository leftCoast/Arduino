#ifndef phone_h
#define phone_h


#include <drawObj.h>
#include <label.h>
#include "panel.h"

class phone;

class phoneBtn :  public drawObj {

   public:
          phoneBtn(int x,int y,char inKey,phone* inPhone);
  virtual ~phoneBtn(void);

  virtual void  drawSelf(void);
  virtual void  doAction(void);

          char    mKeystroke[2];
          phone*  mPhone;
};


class phone : public panel {

  public:
          phone(void);
  virtual ~phone(void);

  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
          void  keystroke(char inKey);
          void  addChar(char inKey);
          void  deleteChar(void);
          void  formatPN(void);
          void  formatOne(void);
          void  formatStar(void);
          void  formatHash(void);
          void  formatStd(void);
          void  call(void);
          void  hangup(void);

          char* mRawPN;
          char* mFormattedPN;

          phoneBtn* pBtndel;
                
          phoneBtn* pBtn7;
          phoneBtn* pBtn8;
          phoneBtn* pBtn9;
                
          phoneBtn* pBtn4;
          phoneBtn* pBtn5;
          phoneBtn* pBtn6;
                
          phoneBtn* pBtn1;
          phoneBtn* pBtn2;
          phoneBtn* pBtn3;
                
          phoneBtn* pBtn0;
          phoneBtn* pBtnStar;
          phoneBtn* pBtnHash;
                
          phoneBtn* pBtnCall;
          phoneBtn* pBtnClose;

          label*    numDisplay;
};

#endif
