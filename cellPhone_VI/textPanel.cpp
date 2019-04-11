#include "cellOS.h"
#include "textPanel.h"
#include <cellCommon.h>
#include <cellManager.h>
#include <resizeBuff.h>
#include <textView.h>


#define TW_X    10
#define TW_Y    MENU_BAR_H
#define TW_W    240 - (2*TW_X)
#define TW_H    340 - MENU_BAR_H - ROW_1 - 10

#define ET_X    TW_X
#define ET_Y    195
#define ET_W    TW_W
#define ET_H    15

#define ML_X    TW_X
#define ML_Y    TW_Y
#define ML_W    TW_W
#define ML_H    TW_H

#define TB_X    0
#define TB_Y    5
#define TB_W    ML_W - 10
#define TB_H    ML_H - 10


contact* currentContact;        



// *****************************************************
// ********************  msgItem  **********************
// *****************************************************

msgItem::msgItem(char* msg,bool us)
  : drawGroup(ML_X,ML_Y,ML_W,ML_H) {

  textView* ourText;
  
  if (us) {
    ourText = new textView(TB_X+10,TB_Y,TB_W,TB_H);
    ourText->setTextColors(&textActiveColor,&backColor);
  } else {
    ourText = new textView(TB_X,TB_Y,TB_W,TB_H);
    ourText->setTextColors(&textColor,&backColor);
  }
  ourText->setText(msg);
  addObj(ourText);
}


msgItem::~msgItem(void) {  }

  
void msgItem::drawSelf(void) { screen->drawRect(this,&white); }



// *****************************************************
// ********************  textKBD  **********************
// *****************************************************


textKBD::textKBD(textPanel* inPanel)
  :keyboard(NULL) { mPanel = inPanel; }

          
textKBD::~textKBD(void) {  }
  

void textKBD::handleKey(keyCommands inEditCom) {

  int   numBytes;
  char* msgBuff;
  
  if (inEditCom == enter) {                           // Got the return key.
    msgBuff = NULL;                                   // Init msgBuff
    numBytes = mEditField->getNumChars() + 1;         // How big is this new message?
    if (resizeBuff(numBytes,(uint8_t**)*msgBuff)) {   // If we got the room..
      mEditField->getText(msgBuff);                   // Grab the message from the edit field.
      mEditField->setValue("");                       // Clear the editField.
      mPanel->addMsg(msgBuff,true);                   // Send in our message.
      resizeBuff(0,(uint8_t**)*msgBuff);              // Recycle reuse..
    } else {
      mEditField->setValue("*** MEMORY ERROR ***");   // Tell the workd, we're dyin!
    }
  } else {
    keyboard::handleKey(inEditCom);                   // Or, do the default suff.
  }
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

  rect          aRect;
    
  if (currentContact) {
    mMsgList = new scrollingList(ML_X,ML_Y,ML_W,ML_H,touchScroll,dragEvents);
    mKeyboard = new textKBD(this);
    aRect.setRect(ET_X,ET_Y,ET_W,ET_H);
    cellEditField* ourEditField  = new cellEditField(&aRect,"",mKeyboard);
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
      if (resizeBuff(buffBytes,(uint8_t**)*msgBuff)) {
        if (currentContact->readMsgBlock(msgBuff,buffBytes)) {
          fillMsgList();
        }
        resizeBuff(0,(uint8_t**)*msgBuff);
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
    newItem = new msgItem(&(msgBuff[index]),us);
    mMsgList->addObj(newItem);
    index = index+chars+1;
  }
}


// A new message has come in. Either we just typed it (us) or it came in
// from the phone (them). We need to create a list item and add it to our
// contact. Adding it to the contact should save it to disk as well.
void textPanel::addMsg(char* msg,bool us) {

  msgItem*  newItem;
  
  newItem = new msgItem(msg,us);
  mMsgList->addObj(newItem);
  currentContact->addMsg(msg,us);
}


void textPanel::loop(void) { }


void textPanel::drawSelf(void) {

  rect  userRect(0,24,240,320-24);

  screen->fillRect(&userRect,&backColor);
}


void textPanel::closing(void) { }
