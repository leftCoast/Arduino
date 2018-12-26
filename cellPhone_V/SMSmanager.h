#ifndef SMSmanager_h
#define SMSmanager_h

#include "keyboard.h"
#include "keystroke.h"
#include "textView.h"
#include "quickCom.h"


class SMSmanager : public keyboard {

  public:
          SMSmanager(editField* inEditField,textView* inTextField);
          ~SMSmanager(void);

  virtual void  handleKey(keyCommands inEditCom);
          bool  resizeOutBuff(byte numBytes);
  
          textView* mTextField;
          char*     mOutBuff;   // The out going message.
          byte      mNumBytes;  // How many bites is it now?
};

#endif
