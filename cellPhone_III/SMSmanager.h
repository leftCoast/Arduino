#ifndef SMSmanager_h
#define SMSmanager_h

#include "keyboard.h"
#include "keystroke.h"


class SMSmanager : public keyboard {

  public:
          SMSmanager(editField* inEditField,label* inTextField);
          ~SMSmanager(void);

  virtual void  handleKey(keyCommands inEditCom);

          label*  mTextField;
};

#endif
