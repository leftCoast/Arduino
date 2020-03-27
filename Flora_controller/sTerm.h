#ifndef sTerm_h
#define sTerm_h

#include <panel.h>
#include <litlOS.h> 
#include <bmpObj.h>
#include <textView.h>
#include "tools.h"



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
//                      sTermPanel
// *****************************************************


class sTermPanel : public panel {

  public:
          sTermPanel(void);
  virtual ~sTermPanel(void);
          
  virtual void  setup(void);
  virtual void  drawSelf(void);
  virtual void  loop(void);
  virtual void  closing(void);
  
          textView* ourScreen;
          char*     mTextBuff;
          int       mBuffBytes;
};

#endif
