#ifndef textPanel_h
#define textPanel_h

#include <drawObj.h>
#include <label.h>
#include <editLabel.h>
#include <scrollingList.h>
#include <panel.h>
#include <menuBar.h>
#include <contactList.h>
#include <colorRect.h>
#include <keystroke.h>
//#include <IOandKeys.h>
#include <bmpKeyboard.h>
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
          msgItem(char* msg,bool us,drawList* inDrawList,bool send=false);
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


class msgEditField  : public cellEditField {

	public:
				msgEditField(rect* inRect,keyboard* inKeyboard,textPanel* inPanel);
	virtual	~msgEditField(void);
  
	virtual	void	idle(void);
  
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

  virtual void  setText(char* text,bool send=false);     
  virtual void  idle(void);
  
          int   mCmdID;   
};


// *****************************************************
// *******************  textPanel  *********************
// *****************************************************


class textPanel  :  public cellOSPanel {

  public:
          textPanel(void);
  virtual ~textPanel(void);
           
  virtual void      setup(void);
          void      fillMsgList(void);
          void      addMsg(char* msg,bool us,bool send=false);
          void      deleteMsgs(void);
          void      checkCommands(void);
          void      checkMessages(void);
  virtual void      loop(void);
  virtual void      drawSelf(void);
  virtual void      closing(void);

          //keyboard* mKeyboard;
          bmpKeyboard*  mKeyboard;
          label*        nickname;
          msgList*      mMsgList;
          char*         msgBuff;
          int           buffBytes;
          int           mCmdID;
          bool          mPNSet;
};


#endif
