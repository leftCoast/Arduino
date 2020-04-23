 #ifndef sTerm_h
#define sTerm_h

#include <panel.h>
#include <idlers.h>
#include <litlOS.h> 
#include <bmpObj.h>
#include <textView.h>
#include "tools.h"
#include "textBuff.h"

class sTermPanel;

// *****************************************************
//                      sKeyboard
// *****************************************************


class sKeyboard : public ourKeyboard {

  public:

                sKeyboard(editLabel* inEditLabel,textView* inTextView,bool modal);
  virtual       ~sKeyboard(void);

  virtual void  handleMessage(char* buff);
  virtual void  handleKey(keyCommands inEditCom);

          textView*   mTextView;
          editLabel*  mEditLabel;         
};


// *****************************************************
//                      sTermUpdater
// *****************************************************

class sTermUpdater : public idler {

  public:
            sTermUpdater(sTermPanel* inPanel);
  virtual   ~sTermUpdater(void);

  virtual  void  idle(void);

            sTermPanel* mPanel;
};

// *****************************************************
//                      sTermPanel
// *****************************************************


class sTermPanel :  public panel {

  public:
          sTermPanel(void);
  virtual ~sTermPanel(void);
          
  virtual void  setup(void);
  virtual void  drawSelf(void);
  virtual void  loop(void);
  virtual void  closing(void);
  
          textView*     ourScreen;
          textBuff*     mReplyBuff;
          sTermUpdater* mUpdater;
};

#endif
