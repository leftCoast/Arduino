#ifndef SMSmanager_h
#define SMSmanager_h

#include "keyboard.h"
#include "keystroke.h"
#include "textView.h"
#include "quickCom.h"

class SMSmanager : public keyboard {

  public:
          SMSmanager(editField* inEditField,textView* inTextField,qCMaster* inComObj);
          ~SMSmanager(void);

  virtual void  handleKey(keyCommands inEditCom);

          textView* mTextField;
          qCMaster* mComObj;
};

#endif
