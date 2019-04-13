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
// ******************  trashMsgBtn  ********************
// *****************************************************


class trashMsgBtn : public trashBtn {

  public:
          trashMsgBtn(int x,int y,textPanel* inPanel);
  virtual ~trashMsgBtn(void);

  virtual void  doAction(void);
  
          textPanel*  mPanel;
};



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
// *****************  sendTextView  ********************
// *****************************************************


class sendTextView  : public textView,
                      public idler  {

  public:
          sendTextView(int inLocX, int inLocY, word inWidth,word inHeight,eventSet inEventSet=noEvents);
  virtual ~sendTextView(void);

  virtual void  setText(char* text);                        // Replace our text buff with a copy of this. 
  virtual void  idle(void);
  
          int   mCmdID;   
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
          void      deleteMsgs(void);
  virtual void      loop(void);
  virtual void      drawSelf(void);
  virtual void      closing(void);

          keyboard* mKeyboard;
          label*    nickname;
          msgList*  mMsgList;
          char*     msgBuff;
          int       buffBytes;
          int       mCmdID;
          bool      mPNSet;
};


#endif
