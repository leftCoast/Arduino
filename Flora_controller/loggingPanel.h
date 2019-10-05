#ifndef loggingPanel_h
#define loggingPanel_h

#include <panel.h>
#include <litlOS.h> 
#include <bmpObj.h> 
#include <stateTracker.h>
#include <flasher.h>
#include <liveText.h>
#include "tools.h"


// *****************************************************
//                   enableLogBtn
// *****************************************************


class enableLogBtn :  public bmpFlasher,
                      public onlineIntStateTracker {

    public:
            enableLogBtn(int x, int y,int width, int height);
    virtual ~enableLogBtn(void);
    
            void  setTheLook();
    virtual void  readState(void);
    virtual void  doAction(void);
    virtual void  idle(void);
};


// *****************************************************
//                   deleteLogBtn
// *****************************************************

class deleteLogBtn :  public iconButton {

    public:
            deleteLogBtn(int x, int y);
    virtual ~deleteLogBtn(void);
    
    virtual void  doAction(void);
};


// *****************************************************
//                   copyLogBtn
// *****************************************************

class copyLogBtn :  public iconButton,
                    public idler {

    enum  transferState { xferIdle, xferStarted, xferRunning };
    
    public:
            copyLogBtn(int x, int y,ourKeyboard* akeyboard,panel* ourPanel);
    virtual ~copyLogBtn(void);

    virtual void  startFileTransfer(void);
    virtual void  deleteEditor(void);
    virtual void  doAction(void);
    virtual void  idle(void);

            transferState mXferState;
            editLabel*    mEditLabel;
            ourKeyboard*  mKeyboard;
            panel*        mPanel;
            timeObj       uiTimer;
};



// *****************************************************
//                      loggingPanel
// *****************************************************


class loggingPanel :  public panel {

  public:
          loggingPanel(void);
  virtual ~loggingPanel(void);
          
  virtual void  setup(void);
  virtual void  drawSelf(void);
          void  msg(char* text);
          
          liveText* mMessage;
};


#endif
