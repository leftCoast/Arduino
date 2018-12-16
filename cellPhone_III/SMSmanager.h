#ifndef SMSmanager_h
#define SMSmanager_h

#include "keyboard.h"
#include "keystroke.h"
#include "textView.h"

class SMSmanager : public keyboard {

  public:
          SMSmanager(editField* inEditField,textView* inTextField);
          ~SMSmanager(void);

  virtual void  handleKey(keyCommands inEditCom);

          textView*  mTextField;
};

#endif
