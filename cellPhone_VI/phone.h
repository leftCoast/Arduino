#ifndef phone_h
#define phone_h


#include <drawObj.h>
#include <label.h>
#include "panel.h"
#include "icons.h"

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

          timeObj statTimer;
          
          void  keystroke(char inKey);
          void  startCall(void);
          void  startHangup(void);
          void  addChar(char inKey);
          void  deleteChar(void);
          void  formatPN(void);
          void  formatOne(void);
          void  formatStar(void);
          void  formatHash(void);
          void  formatStd(void);
          void  doCall(void);
          void  doHangup(void);

          char* mRawPN;
          char* mFormattedPN;
          bool  mNeedToCall;    // We need to be running through the calling sequence.
          bool  mConnected;     // Successful, as far as we can tell, calling sequence.
          bool  mNeedToHangup;  // We need to be running through the hanging up sequence.
          bool  mNeedClose;

          
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
          label*    statDisplay;
          
          battPercent*  mBatPct;
          RSSIicon*     mRSSI;
          bool          mSeenStatus;
};

#endif
