#ifndef phone_h
#define phone_h


#include <drawObj.h>
#include <label.h>
#include <liveText.h>
#include <PNLabel.h>
#include <colorRect.h>
#include <contactList.h>
#include "panel.h"
#include "../../icons.h"

extern	contact* pleaseCall;

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


class callControl : public phoneBtn,
                    public idler {
    
  public:

  enum callConState { isIdle, hasIncoming, isConnected };
  
          callControl(int x,int y,char inKey,phone* inPhone);
  virtual ~callControl(void);

  virtual void  drawSelf(void);
  virtual void  doAction(void);
  virtual void  idle();

          callConState  mState;
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
          void  addChar(char inKey);
          void  deleteChar(void);
          void  answerCall(void);
          void  startCall(void);
          void  startHangup(void);
          void  checkCall(void);
          void  checkHangup(void);
          void  out(char* message);
          void  out(int message);
          
          char* mRawPN;
          int   mCallingID;     // We're in the calling sequence. Here's our command ID.
          bool  mConnected;     // Successful, as far as we can tell, calling sequence.
          int   mHangupID;      // We're in the hangup sequence. Here's our command ID.
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

          colorRect*  theEditBase;     // Background of the PNLabel.
          PNLabel*  numDisplay;
          liveText* stateDisplay;
          
          battPercent*  mBatPct;
          RSSIicon*     mRSSI;
          bool          mSeenStatus;
};

#endif
