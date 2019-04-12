#include "cellOS.h"
#include "textPanel.h"
#include <cellCommon.h>
#include <cellManager.h>
#include <resizeBuff.h>
#include <textView.h>


#define TW_X    10
#define TW_Y    MENU_BAR_H + 5
#define TW_W    240 - (2*TW_X)
#define TW_H    160

#define ET_X    TW_X
#define ET_Y    195
#define ET_W    TW_W
#define ET_H    15

#define ML_X    TW_X
#define ML_Y    TW_Y
#define ML_W    TW_W
#define ML_H    TW_H

#define MI_X    0
#define MI_Y    0
#define MI_W    ML_W
#define MI_H    32

#define TV_X    5
#define TV_Y    5
#define TV_W    ML_W-10
#define TV_H    MI_H - 10

contact* currentContact;        



// *****************************************************
// ********************  msgItem  **********************
// *****************************************************

msgItem::msgItem(char* msg,bool us,drawList* inDrawList)
  : drawGroup(MI_X,MI_Y,MI_W,MI_H) {
  mDrawList = inDrawList;
  if (us) {
    mText = new textView(TV_X,TV_Y,TV_W,TV_H);
    mText->setTextColors(&textActiveColor,&backColor);
  } else {
    mText = new textView(TV_X,TV_Y,TV_W,TV_H);
    mText->setTextColors(&textColor,&backColor);
  }
  mText->setText(msg);
  addObj(mText);
}


msgItem::~msgItem(void) {  }


void msgItem::draw(void) {

  if (mDrawList->isVisible(this)) {
      drawGroup::draw();
  }
  needRefresh = false;
}

  
void msgItem::drawSelf(void) { }



// *****************************************************
// ********************  msgList  **********************
// *****************************************************

// The ONLY reason to have this is to mask the default drawing.
msgList::msgList(void)
  : scrollingList(ML_X,ML_Y,ML_W,ML_H,touchScroll,dragEvents) { }

  
msgList::~msgList(void) { }


void msgList::drawSelf(void) {  }



// *****************************************************
// *****************  msgEditField  ********************
// *****************************************************


msgEditField::msgEditField(textPanel* inPanel)
  : editField(1,1,1,1,"",1) { mPanel = inPanel; }

  
msgEditField::~msgEditField(void) {  }

  
void msgEditField::handleKeystroke(keystroke* aKeystroke) {
  
  int   numBytes;
  char* msgBuff;

  if (aKeystroke->theChar=='\0') {
    if (aKeystroke->editCommand==enter) {
      msgBuff = NULL;                                   // Init msgBuff
      numBytes = getNumChars() + 1;                     // How big is this new message?
      if (resizeBuff(numBytes,(uint8_t**)&msgBuff)) {   // If we got the room..
        getText(msgBuff);                               // Grab our message.
        setValue("");                                   // Clear the message.
        mPanel->addMsg(msgBuff,true);                   // Send in our message.
        resizeBuff(0,(uint8_t**)&msgBuff);             // Recycle reuse..
        return;                                         // In this case? We're done, lets go!
      }
    }
  }
  editField::handleKeystroke(aKeystroke);              // In every other case we let the inherited do its thing.
}



// *****************************************************
// ******************  textPanel  *******************
// *****************************************************


textPanel::textPanel(void)
  : panel(textApp,noEvents) {
  
  mKeyboard = NULL;
  msgBuff = NULL;
 }


textPanel::~textPanel(void) { if (mKeyboard) delete mKeyboard; }

           
void textPanel::setup(void) {

  rect  aRect;
    
  if (currentContact) {
    mMsgList = new msgList();
    addObj(mMsgList);
    
    mKeyboard = new keyboard(NULL);
    
    msgEditField* ourMsgEditField = new msgEditField(this);
    aRect.setRect(ET_X,ET_Y,ET_W,ET_H);
    cellEditField* ourEditField  = new cellEditField(&aRect,"",mKeyboard,ourMsgEditField);
    if (ourEditField) { 
      setFocusPtr(ourEditField);
      addObj(ourEditField);
    }
    
    menuBar* ourMenuBar = new menuBar((panel*)this);
    addObj(ourMenuBar);
    
    contactsBtn* ourContactsBtn = new contactsBtn(CLOSE_X+40,CLOSE_Y);
    ourMenuBar->addObj(ourContactsBtn);
    
    label* nickname = new label(CLOSE_X+80,7,100,22,currentContact->mNickName,1);
    nickname->setJustify(TEXT_CENTER);
    nickname->setColors(&textColor,&menuBarColor);
    ourMenuBar->addObj(nickname);
    
    buffBytes = currentContact->sizeOfMsgBlock();
    if (buffBytes) {
      if (resizeBuff(buffBytes,(uint8_t**)&msgBuff)) {
        if (currentContact->readMsgBlock(msgBuff,buffBytes)) {
          fillMsgList();
        }
        resizeBuff(0,(uint8_t**)&msgBuff);
        buffBytes = 0;
      }
    }
    
  } else {
    nextPanel = contactApp;
  }
}


// We have a block of messages from our contact. Break it down and fill
// the list with all these messages.
void textPanel::fillMsgList(void) {

  int       index;
  int       chars;
  bool      us;
  msgItem*  newItem;
  
  index = 0;
  while(index<buffBytes) {
    us = msgBuff[index]=='U';
    index++;
    chars = strlen(&(msgBuff[index]));
    newItem = new msgItem(&(msgBuff[index]),us,mMsgList);
    mMsgList->addObj(newItem);
    index = index+chars+1;
  }
}


// A new message has come in. Either we just typed it (us) or it came in
// from the phone (them). We need to create a list item and add it to our
// contact. Adding it to the contact should save it to disk as well.
void textPanel::addMsg(char* msg,bool us) {

  msgItem*  newItem;
  
  newItem = new msgItem(msg,us,mMsgList);
  mMsgList->addObj(newItem);
  mMsgList->setPositions();
  mMsgList->showItem(newItem);
  currentContact->addMsg(msg,us);
}


void textPanel::loop(void) { }


void textPanel::drawSelf(void) {

  rect  userRect(0,24,240,320-24);

  screen->fillRect(&userRect,&backColor);
}


void textPanel::closing(void) { }
