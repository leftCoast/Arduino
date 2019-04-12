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

class textPanel;


// *****************************************************
// ********************  msgItem  **********************
// *****************************************************


class msgItem : public drawGroup {
  
  public:
          msgItem(char* msg,bool us,drawList* inDrawList);
  virtual ~msgItem(void);

  virtual void  draw(void);
  virtual void  drawSelf(void);
    
    drawList* mDrawList;
    textView* mText;
};



// *****************************************************
// ********************  msgList  **********************
// *****************************************************


class msgList : public scrollingList {

  public:
          msgList(void);
  virtual ~msgList(void);

  virtual void  drawSelf(void);      
};


// *****************************************************
// *****************  msgEditField  ********************
// *****************************************************


class msgEditField  : public editField {

  public:
          msgEditField(textPanel* inPanel);
  virtual ~msgEditField(void);
  
  virtual void  handleKeystroke(keystroke* aKeystroke);
  
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

          keyboard* mKeyboard;
          msgList*  mMsgList;
          char*     msgBuff;
          int       buffBytes;
};


#endif
