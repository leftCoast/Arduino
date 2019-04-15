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
// ******************  trashMsgBtn  ********************
// *****************************************************


trashMsgBtn::trashMsgBtn(int x,int y,textPanel* inPanel)
  : trashBtn(x,y) { mPanel = inPanel; }

  
trashMsgBtn::~trashMsgBtn(void) {  }

void trashMsgBtn::doAction(void) { mPanel->deleteMsgs(); }



// *****************************************************
// ********************  msgItem  **********************
// *****************************************************


msgItem::msgItem(char* msg,bool us,drawList* inDrawList,bool send)
  : drawGroup(MI_X,MI_Y,MI_W,MI_H) {
  sendTextView* sendText;
   
  mDrawList = inDrawList;
  if (us) {
    sendText = new sendTextView(TV_X+40,TV_Y,TV_W-40,TV_H);
    if (send) {
      sendText->setTextColors(&textSelectColor,&backColor);
    } else {
      sendText->setTextColors(&green,&backColor);
    }
    sendText->setText(msg,send);
    mText = (textView*)sendText;
  } else {
    mText = new textView(TV_X,TV_Y,TV_W-40,TV_H);
    mText->setTextColors(&textColor,&backColor);
    mText->setText(msg);
  }
  addObj(mText);
}


msgItem::~msgItem(void) {  }


void msgItem::draw(void) {

  if (mDrawList->isVisible(this)) {
      drawGroup::draw();
  }
  needRefresh = false;
}

  
void msgItem::drawSelf(void) { screen->fillRect(this,&backColor); }



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
    if (aKeystroke->editCommand==enter) {                 // We are going to handle ANY click on enter.
      if (mPanel->mPNSet) {                               // If we've not been able to set the phone number, don't bother.
        msgBuff = NULL;                                   // Init msgBuff
        numBytes = getNumChars() + 1;                     // How big is this new message?
        if (resizeBuff(numBytes,(uint8_t**)&msgBuff)) {   // If we got the room..
          getText(msgBuff);                               // Grab our message.
          setValue("");                                   // Clear the message.
          mPanel->addMsg(msgBuff,true,true);              // Send in our message.
          resizeBuff(0,(uint8_t**)&msgBuff);              // Recycle reuse..
        }
      }
      return;                                             // Enter handled, time to go.
    }
  }
  editField::handleKeystroke(aKeystroke);                 // In every other case we let the inherited do its thing.
}



// *****************************************************
// *****************  sendTextView  ********************
// *****************************************************


sendTextView::sendTextView(int inLocX, int inLocY, word inWidth,word inHeight,eventSet inEventSet)
  : textView(inLocX,inLocY,inWidth,inHeight,inEventSet) {

  mCmdID = -1;
  hookup();
}

  
sendTextView::~sendTextView(void) { }


void sendTextView::setText(char* text,bool send) {
  
  textView::setText(text);
  if (send) {
    mCmdID = ourCellManager.sendCommand(sendSMS,text,true);
  }
}


void  sendTextView::idle(void) {

  byte  numBytes;
  byte  answer;
  
  if (mCmdID!=-1) {                                         // If we have a valid command ID, and phone number is not set..
    switch (ourCellManager.progress(mCmdID)) {              // Check status of our command ID and take appopriate action..
      case com_standby  : 
      case com_working  : break;                            // Standby or working means its in the queue or in process. Come back later.
      case com_holding  :                                   // Holding means we have some sort of answer.
        numBytes = ourCellManager.numReplyBytes(mCmdID);    // Read the number of bytes we have being held for us.
        if (numBytes==1) {                                  // We only are interested if its one byte. Otherwise something's broken somewhere.
          ourCellManager.readReply(mCmdID,&answer);         // It was one byte, lets pick it up.
          if (answer==0) {                                  // The answer we want is 0, meaning 0 errors.
            setTextColors(&green,&backColor);
            currentContact->addMsg(seeText(),true);
            mCmdID = -1;
            break;
          }
        }
        setTextColors(&red,&backColor);                     // If we got here, its broken.
        //setText("Got back error.");
        mCmdID = -1;                                        // Errors or not, we are done with it.
      break;
      case com_complete      :       
      case com_missing       :
        setTextColors(&red,&backColor);                     // And, if we got here, its broken. 
        //setText("Command timed out.");
        mCmdID = -1;                                        // Done with it, I think something broke.
      break;                                                
    }
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

    
    trashMsgBtn* ourTrashMsgBtn = new trashMsgBtn(CLOSE_X+172,CLOSE_Y,this);
    ourMenuBar->addObj(ourTrashMsgBtn);
    
    nickname = new label(CLOSE_X+75,7,100,22,currentContact->mNickName,1);
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
    mPNSet = false;
    mCmdID = ourCellManager.sendCommand(setCurrentPN,currentContact->mPN,true);
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
// contact. Adding one of ours (us) to the list item, starts the sending
// sequence. Once its confirmed sent, the list itme will send it to the
// contact to save on disk.
void textPanel::addMsg(char* msg,bool us,bool send) {

  msgItem*  newItem;
  
  newItem = new msgItem(msg,us,mMsgList,send);
  mMsgList->addObj(newItem);
  mMsgList->setPositions();
  mMsgList->showItem(newItem);
}


void textPanel::deleteMsgs(void) {

  currentContact->deleteMsgBlock();
  mMsgList->dumpDrawObjList();
  screen->fillRect(mMsgList,&backColor);
}


// We run a lot of commands. Lets see if they need service.
void  textPanel::checkCommands(void) {

  byte  numBytes;
  byte  answer;
  
  if (mCmdID!=-1 && !mPNSet) {                              // If we have a valid command ID, and phone number is not set..
    switch (ourCellManager.progress(mCmdID)) {              // Check status of our command ID and take appopriate action..
      case com_standby  : 
      case com_working  : break;                            // Standby or working means its in the queue or in process. Come back later.
      case com_holding  :                                   // Holding means we have some sort of answer.
        numBytes = ourCellManager.numReplyBytes(mCmdID);    // Read the number of bytes we have being held of us.
        if (numBytes==1) {                                  // We only are interested if its one byte. Otherwise something had broken somewhere.
          ourCellManager.readReply(mCmdID,&answer);         // It was one byte, lets pick it up.
          if (answer==0) {                                  // The answer we want is 0, meaning 0 errors.
            nickname->setColors(&green,&menuBarColor);
            mPNSet = true;                                  // Ok, set the flag that the phone number is set.
            break;
          }
        }
        nickname->setColors(&red,&menuBarColor);            // If we got here, its broken.
        mCmdID = -1;                                        // Errors or not, we are done with it.
      break;
      case com_complete      :        
      case com_missing       :
        nickname->setColors(&red,&menuBarColor);            // And, if we got here, its broken. 
        mCmdID = -1;                                        // Done with it, I think something broke.
      break;                                                
    }
  }
}


// While were doing the text message thing, check the incoming messages to see if we should display them.
void textPanel::checkMessages(void) { 
  
  if (newSMSmsg) {                              // We have a new message to check?
    if (!strcmp(SMSPN,currentContact->mPN)) {   // Is it ours?
      addMsg(SMSMsg,false);                     // Add it to our list as theirs.
    }
    newSMSmsg = false;                          // Seen it, we're done.
  }
}


void textPanel::loop(void) {
 
  checkCommands();
  checkMessages();
}


void textPanel::drawSelf(void) {

  rect  userRect(0,24,240,320-24);

  screen->fillRect(&userRect,&backColor);
}


void textPanel::closing(void) { }
