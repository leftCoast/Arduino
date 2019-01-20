
#include "phone.h"
#include <cellCommon.h>
#include <drawDelete.h>
#include "cellManager.h"
#include	"../../cellOS.h"
#include "../cellListener/cellListener.h"

#define STAT_TIME   2500  // ms between checks on status.

#define KEY_W       40
#define KEY_H       30

#define ROW_GAP     50

#define KEY_R0      30
#define KEY_R1      KEY_R0 + ROW_GAP
#define KEY_R2      KEY_R1 + ROW_GAP
#define KEY_R3      KEY_R2 + ROW_GAP
#define KEY_R4      KEY_R3 + ROW_GAP
#define KEY_R5      KEY_R4 + ROW_GAP

#define COL_GAP     70

#define KEY_C1      30
#define KEY_C2      KEY_C1 + COL_GAP
#define KEY_C3      KEY_C2 + COL_GAP

#define EB_X    KEY_C1
#define EB_Y    KEY_R0
#define EB_W    KEY_W+COL_GAP+20
#define EB_H    16

#define ET_INSET  3                     
#define ET_X      EB_X+ET_INSET
#define ET_Y      EB_Y+4
#define ET_W      EB_W-(2*ET_INSET)
#define ET_H      10

#define ST_X      ET_X
#define ST_Y      ET_Y + 14
#define ST_W      ET_W
#define ST_H      ET_H

#define KEY_RAD     4

#define TEXT_SIZE   2
#define CHAR_WIDTH  TEXT_SIZE * 6
#define TEXT_HEIGHT TEXT_SIZE * 8

#define BATT_X    199
#define BATT_Y    4

#define SIG_X     BATT_X + 15
#define SIG_Y     BATT_Y


// *****************************************************
// ********************  phoneBtn  *********************
// *****************************************************


phoneBtn::phoneBtn(int x,int y,char inKey,phone* inPhone)
  : drawObj(x,y,KEY_W,KEY_H,true) {
  
  mKeystroke[0] = inKey;
  mKeystroke[1] = '\0';
  mPhone = inPhone;
  mPhone->addObj(this);    // If we add to our phone object, we'll be auto deleted when its recycled.
}


phoneBtn::~phoneBtn(void) {  }


void phoneBtn::drawSelf(void) {

  int         inset = 5;                      // All the hand tweaking to
  int         arrowX1 = x+inset+3;            // setup the delete key arrow.
  int         arrowY1 = y+inset-1;            // After all that, Julie insists it
  int         arrowX2 = width-(2*(inset+3));  // Looks like an empty Batt. icon.
  int         arrowY2 = height-(2*inset);
  rect        arrowRect(arrowX1,arrowY1,arrowX2,arrowY2);
  drawDelete  arrow(&arrowRect);

    
  if (mKeystroke[0]=='X') {
    if (clicked) {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&backColor);
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
      screen->setTextColor(&redButtonColor);
    } else {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&redButtonHighlight);
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&redButtonColor);
      screen->setTextColor(&black);
    }
  } else {
    if (clicked) {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&lightbButtonColorHit);
      screen->setTextColor(&white);
      arrow.setColors(&redButtonColor,&lightbButtonColorHit);
    } else {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&lightbButtonColorHit);  // Gives a cool highlight.
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&lightbButtonColor);
      screen->setTextColor(&textSelectColor);
      arrow.setColors(&redButtonColor,&lightbButtonColor);
    }
  }
  if (mKeystroke[0]=='D') {
    arrow.stamp();
  } else {
    screen->setTextSize(TEXT_SIZE);
    screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
    screen->drawText(mKeystroke);
  }
}


void phoneBtn::doAction(void) { mPhone->keystroke(mKeystroke[0]); }


          
// *****************************************************
// *******************  callControl  *******************
// *****************************************************

  
callControl::callControl(int x,int y,char inKey,phone* inPhone)
  : phoneBtn(x,y,inKey,inPhone) {

  width = width + COL_GAP;  // All the buttons are smaller, we're "special".
  mState = isIdle;
}


callControl::~callControl(void) { }


void callControl::drawSelf(void) {

  hookup();
  screen->setTextSize(TEXT_SIZE);
  switch(mState) {
    case isIdle       :
      if (clicked) {
        screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&backColor);
        screen->fillRoundRect(x,y,width,height,KEY_RAD,&lightbButtonColor);
        screen->setTextColor(&green);
      } else {
        screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&greenButtonHighlight);
        screen->fillRoundRect(x,y,width,height,KEY_RAD,&greenbuttonColor);
        screen->setTextColor(&white);
      }
      screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
      screen->drawText("Call");
    break;
    case hasIncoming  :
      if (clicked) {
        screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
        screen->setTextColor(&green);
      } else {
        screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&greenButtonHighlight);
        screen->fillRoundRect(x,y,width,height,KEY_RAD,&greenbuttonColor);
        screen->setTextColor(&white);
      }
      screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
      screen->drawText("Answer");
      mPhone->out("Click X to refuse call.");
    break;
    case isConnected  :
      if (clicked) {
        screen->fillRoundRect(x,y,width,height,KEY_RAD,&backColor);
        screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
        screen->setTextColor(&redButtonColor);
      } else {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&redButtonHighlight);
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&redButtonColor);
      screen->setTextColor(&white);
      }
      screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
      screen->drawText("Hangup");
    break;   
  }
}


void callControl::doAction(void) {

  switch(mState) {
    case isIdle       : mPhone->startCall();    break;
    case hasIncoming  : mPhone->answerCall(); break;
    case isConnected  : mPhone->startHangup();  break;
  }
}


void callControl::idle() {

  switch(mState) {
    case isIdle       : 
      if(callIncoming) {
        mState = hasIncoming;
        setNeedRefresh();
      } else if (mPhone->mConnected) {
        mState = isConnected;
        setNeedRefresh();
      }
    break;
    case hasIncoming  :
      if (mPhone->mConnected) {
        mState = isConnected;
        setNeedRefresh();
      }
    break;
    case isConnected  :
      if (!mPhone->mConnected) {
        mState = isIdle;
        setNeedRefresh();
      }
    break;
  }
}



          
// *****************************************************
// *********************  phone   **********************
// *****************************************************


phone::phone(void) 
  : panel(phoneApp,false) {

  mRawPN          = NULL;
  mCallingID      = -1;
  mConnected      = false;
  mHangupID       = -1;
  mNeedClose      = false;
  mSeenStatus     = false;
}


phone::~phone(void) { if (mRawPN) free(mRawPN); }


void phone::setup(void) {

  statTimer.setTime(STAT_TIME);
  pBtndel   = new phoneBtn(KEY_C3,KEY_R0,'D',this);
  
  pBtn7     = new phoneBtn(KEY_C1,KEY_R1,'7',this);
  pBtn8     = new phoneBtn(KEY_C2,KEY_R1,'8',this);
  pBtn9     = new phoneBtn(KEY_C3,KEY_R1,'9',this);
  
  pBtn4     = new phoneBtn(KEY_C1,KEY_R2,'4',this);
  pBtn5     = new phoneBtn(KEY_C2,KEY_R2,'5',this);
  pBtn6     = new phoneBtn(KEY_C3,KEY_R2,'6',this);
  
  pBtn1     = new phoneBtn(KEY_C1,KEY_R3,'1',this);
  pBtn2     = new phoneBtn(KEY_C2,KEY_R3,'2',this);
  pBtn3     = new phoneBtn(KEY_C3,KEY_R3,'3',this);
  
  pBtn0     = new phoneBtn(KEY_C1,KEY_R4,'0',this);
  pBtnStar  = new phoneBtn(KEY_C2,KEY_R4,'*',this);
  pBtnHash  = new phoneBtn(KEY_C3,KEY_R4,'#',this);
  
  pBtnCall  = new callControl(KEY_C1,KEY_R5,'C',this);
  pBtnClose = new phoneBtn(KEY_C3,KEY_R5,'X',this);

  theEditBase = new colorRect(EB_X,EB_Y,EB_W,EB_H);
  theEditBase->setColor(&editFieldBColor);
  addObj(theEditBase);
  
  numDisplay  = new PNLabel(ET_X,ET_Y,ET_W,ET_H,1);
  numDisplay->setColors(&textSelectColor,&editFieldBColor);
  addObj(numDisplay);

  stateDisplay  = new liveText(ST_X,ST_Y,ST_W,ST_H,100);
  stateDisplay->addAColor(0,&textColor);
  stateDisplay->addAColor(2500,&textColor);
  stateDisplay->addAColor(3000,&backColor);
  stateDisplay->hold();
  stateDisplay->setColors(&textColor,&backColor);  // Sets the transp variable to false;
  addObj(stateDisplay);
  
  mBatPct = new battPercent(BATT_X,BATT_Y);
  addObj(mBatPct);
  mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor);
  mRSSI   = new RSSIicon(SIG_X,SIG_Y);
  addObj(mRSSI);
}


void phone::loop(void) {

  // Weird hack to get the battery & RSSI things to draw
  // correctly. Why would there be a time issue? How could
  // time be a factor? And as you add more things, you need
  // more time.
  if(!mSeenStatus) {  
    delay(150);                                                 // Calls idle for 150 ms.
    mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor); // Stuff in a value.
    mRSSI->setRSSI(statusReg.RSSI);                             // Ditto.
    mSeenStatus = true;                                         // Done it, lets not come back here again.
  }
  
  if (statTimer.ding()) {
    mBatPct->setPercent((byte)statusReg.batteryPercent,&backColor);
    mRSSI->setRSSI(statusReg.RSSI);
    statTimer.start();
  }
  if (!mConnected && mNeedClose) close();
  checkCall();
  checkHangup();
}


void phone::drawSelf(void) {

  screen->fillScreen(&backColor);
  screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
}


void phone::keystroke(char inKey) {

  switch(inKey) {
    case '0'   :
    case '1'   :
    case '2'   :
    case '3'   :
    case '4'   :
    case '5'   :
    case '6'   :
    case '7'   :
    case '8'   :
    case '9'   : 
    case '*'   :
    case '#'   :
      addChar(inKey);
      numDisplay->setValue(mRawPN);
    break;
    case 'D'   :
      deleteChar();
      numDisplay->setValue(mRawPN);
    break;
    case 'X'   : 
      startHangup();
      mNeedClose = true;
    break;
  }  
}


void phone::addChar(char keyStroke) {
  
  int   numChars;
  char* newStr;
  
  if (!mRawPN) {
    mRawPN = (char*)malloc(2);
    mRawPN[0] = keyStroke;
    mRawPN[1] = '\0';
  } else {
    numChars = strlen(mRawPN);
    newStr = (char*)malloc(numChars+2);
    strcpy(newStr,mRawPN);
    newStr[numChars] = keyStroke;
    newStr[numChars+1] = '\0';
    free(mRawPN);
    mRawPN = newStr;
  }
}


void phone::deleteChar(void) {

  if (mRawPN) {
    if (mRawPN[0]!='\0') {
      mRawPN[strlen(mRawPN)-1] = '\0';
    }
  }
}


void phone::answerCall(void) {

  if (!mConnected) {
    out("Answering..");
    mCallingID = ourCellManager.sendCommand(pickUp,true);
  }
}


void phone::startCall(void) {

  if (!mConnected) {
    out("Calling..");
    mCallingID = ourCellManager.sendCommand(makeCall,mRawPN,true);
  }
}


void phone::startHangup(void) {

  if (mConnected) {
    out("Hanging up..");
    mHangupID = ourCellManager.sendCommand(hangUp,true);
  }
}


void phone::checkCall(void) {

  int           numBytes;
  byte          reply;
  
  if (mCallingID!=-1) {                                       // Working on a call?
    if (ourCellManager.progress(mCallingID)==com_holding) {   // We found it and its holding a reply?
      numBytes = ourCellManager.numReplyBytes(mCallingID);     // This is how big the reply is.
      if (numBytes==1) {                                      // We're looking for a one byte reply.
        ourCellManager.readReply(mCallingID,&reply);          // Grab the byte.
        if (reply==0) {                                       // Is it a zero?
          mConnected = true;                                  // Cool!, we're connected!
          out("Connected.");
        }
      } else {
        ourCellManager.dumpBuff();                            // Unknown what it is. Toss it.
        out("Connect error.");
      }
      mCallingID = -1;                                        // In any case, we're done with the command.
    } else if (ourCellManager.progress(mCallingID)==com_missing) {
      out("Call failed.");
      mCallingID = -1;
    }
  }
}


void phone::checkHangup(void) {

  int   numBytes;
  byte  reply;
  
  if (mHangupID!=-1) {                                      // Working on a call?
    switch (ourCellManager.progress(mHangupID)){            // See what's going on here..
      case com_standby  : 
      case com_working  :  break;                           // Nothing we can do here.
      case com_holding  :                                   // Holding means there's a reply waiting for us.
        numBytes = ourCellManager.numReplyBytes(mHangupID); // This is how big the reply is.
        if (numBytes==1) {                                  // We're looking for a one byte reply.
          ourCellManager.readReply(mHangupID,&reply);       // Grab the byte.
          if (reply==0) {                                   // Is it a zero?
            mConnected = false;                             // Cool!, we're disconnected!
            out("Disconnectd.");                            // Tell the world!
          } else {
            out("Hangup error.");                           // Troubles somewhere.
          }
        } else {
          ourCellManager.dumpBuff();                        // Unknown what it is. Toss it.
          out("Reply error.");                              // Something weird going on..
        }
      break;
      case com_complete :                                   // All this means is it gave us back a reply. (Any reply)                           
      case com_missing  :                                   // Possibly timed out or we already got it.
        mHangupID = -1;
      break;
    }
  }
}


void phone::out(int message) {
  
  stateDisplay->setColors(&textColor,&backColor);
  stateDisplay->setValue(message);
  stateDisplay->release(true);
}


void phone::out(char* message) {
  
  stateDisplay->setColors(&textColor,&backColor);
  stateDisplay->setValue(message);
  stateDisplay->release(true);
}
