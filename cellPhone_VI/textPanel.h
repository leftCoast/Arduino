#ifndef textPanel_h
#define textPanel_h

#include <drawObj.h>
#include <label.h>
#include <editField.h>
#include <scrollingList.h>
#include <panel.h>
#include <contactList.h>
#include <colorRect.h>
#include <keystroke.h>
#include <scrKeyboard.h>

#include "contactPanel.h" // For our edit field. Soon to be moved to cellOS I think.

extern  contact* currentDialog;

 
// Ok, if there is no currentDialog, then we pop over to contacts and select one.
// This panel is only used for actual dialogs with contacts.

// *****************************************************
// *******************  textPanel  *********************
// *****************************************************


class textPanel  :  public panel {

  public:
          textPanel(void);
  virtual ~textPanel(void);
           
  virtual void      setup(void);
  virtual void      loop(void);
  virtual void      drawSelf(void);
  virtual void      closing(void);

          keyboard* mKeyboard;
};



#endif
