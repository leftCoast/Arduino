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
#include <IOandKeys.h>
#include <scrollingList.h>


extern  contact* currentContact;

 
// Ok, if there is no currentContact, then we pop over to contacts and select one.
// This panel is only used for actual dialogs with contacts.

// *****************************************************
// ********************  msgItem  **********************
// *****************************************************


class msgItem : public drawGroup {
  
  public:
          msgItem(char* msg,bool us);
  virtual ~msgItem(void);
  
  virtual void  drawSelf(void);
};



// *****************************************************
// ********************  textKBD  **********************
// *****************************************************
class textPanel;

class textKBD : public keyboard {

  public:
          textKBD(textPanel* inPanel);
  virtual ~textKBD(void);
  
  virtual void  handleKey(keyCommands inEditCom);

          textPanel*  mPanel;
};


// *****************************************************
// *******************  textPanel  *********************
// *****************************************************


class textPanel  :  public panel {

  public:
          textPanel(void);
  virtual ~textPanel(void);
           
  virtual void      setup(void);
          void      fillMsgList(void);
          void      addMsg(char* msg,bool us);
  virtual void      loop(void);
  virtual void      drawSelf(void);
  virtual void      closing(void);

          textKBD*        mKeyboard;
          scrollingList*  mMsgList;
          char*           msgBuff;
          int             buffBytes;
};


#endif
