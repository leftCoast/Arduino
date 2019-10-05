#include "controlPanel.h"
#include <bmpObj.h> 
#include <bmpKeyboard.h> 

// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************


#define LINE_SPACE  20
#define TOP_Y       30
#define LABEL_W     138
#define LABEL_H     18
#define LINE_SPACE  20

#define LIVE_X        LABEL_W+5
#define LIVE_W        90

controlPanel* ourPanel;

controlPanel::controlPanel(void) : panel(controlApp) {  }


controlPanel::~controlPanel(void) { }


void controlPanel::setup(void) {

  int traceY;
  int stepY;
  
  traceY = TOP_Y;
  stepY  = LINE_SPACE;
  
  ourPanel = this;
  
  mKeyboard = new ourKeyboard(NULL,true);
  mKeyboard->loadKeys();
  addObj(mKeyboard);
  
  editName* mNameLabel = new editName(10,traceY,230,18,mKeyboard);
  addObj(mNameLabel);


  traceY = traceY + 2 * stepY;
  bmpObj* limit = new bmpObj(0,traceY,LABEL_W,LABEL_H,LIMIT_BMP);
  limit->begin();
  addObj(limit);

  editLimit* lText = new editLimit(LIVE_X,traceY,LIVE_W,LABEL_H,mKeyboard);
  addObj(lText);
  
  traceY = traceY + stepY;
  bmpObj* waterT = new bmpObj(0,traceY,LABEL_W,LABEL_H,WATER_TIME_BMP);
  waterT->begin();
  addObj(waterT);

  editWTime*  wtText = new editWTime(LIVE_X,traceY,LIVE_W,LABEL_H,mKeyboard);
  addObj(wtText);

  traceY = traceY + stepY;
  bmpObj* soakT = new bmpObj(0,traceY,LABEL_W,LABEL_H,SOAK_TIME_BMP);
  soakT->begin();
  addObj(soakT);

  editSTime*  sText = new editSTime(LIVE_X,traceY,LIVE_W,LABEL_H,mKeyboard);
  addObj(sText);


  mMessage = new liveText(30,9,210,10,100);
  mMessage->setColors(&yellow,&black);
  mMessage->setJustify(TEXT_LEFT);
  mMessage->addAColor(0,&yellow);
  mMessage->addAColor(3000,&yellow);
  mMessage->addAColor(4000,&black);
  mMessage->hold();
  addObj(mMessage);
}


void controlPanel::msg(char* text) {

  mMessage->setValue(text);
  mMessage->release();
}


void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}



// *****************************************************
//                    editName
// *****************************************************


editName::editName(int x,int y,int width,int height,keyboard* inKeyboard)
  : monoNameText(x,y,width,height) {
  
  mKeyboard = inKeyboard;
  setEventSet(fullClick);
}

  
editName::~editName(void) {  }


void editName::endEditing(void) {

  if (ourComPort.getOnline() && mSuccess) {
    ourComPort.setNameReg(buff);
  }
  mKeyboard->setEditField(NULL);            // Release the keyboard.
  setColors(&white,&black);
  monoNameText::endEditing();
  ourComPort.setOnline(false);
}

  
void editName::doAction(event* inEvent,point* locaPt) {

  if (mEditing) {
    ourOS.beep();
    editLabel::doAction(inEvent,locaPt);
  } else {
    if (ourComPort.getOnline() && mKeyboard->mEditObj==NULL) {
      ourPanel->msg("Enter name for this plantBot.");
      ourOS.beep();
      mKeyboard->setEditField(this);
      setColors(&yellow,&black);
      beginEditing();
    } 
  }
}


void editName::idle(void) {

  monoNameText::idle();
  if (!mEditing) {
    if (checkState()) {
      setTheLook();
    }
  }
}



// *****************************************************
//                    editLimit 
// *****************************************************


editLimit::editLimit(int x,int y,int width,int height,keyboard* inKeyboard)
  : moistureLimitText(x,y,width,height) {
  
  mKeyboard = inKeyboard;
  setEventSet(fullClick);
}

  
editLimit::~editLimit(void) {  }


void editLimit::endEditing(void) {

  if (ourComPort.getOnline() && mSuccess) {
    ourComPort.setLimitReg(atoi(buff));
  }
  mKeyboard->setEditField(NULL);            // Release the keyboard.
  setRect(x,y,width+(2 * CHAR_WIDTH * textSize),height);
  setColors(&white,&black);
  moistureLimitText::endEditing();
  setTheLook();
}

  
void editLimit::doAction(event* inEvent,point* locaPt) {

  
  if (mEditing) {
    ourOS.beep();
    editLabel::doAction(inEvent,locaPt);
  } else {
    if (ourComPort.getOnline() && mKeyboard->mEditObj==NULL) {
      ourPanel->msg("Enter limit as % from 0..100.");
      ourOS.beep();
      screen->fillRect(this,&black);
      mKeyboard->setEditField(this);
      setColors(&yellow,&black);
      setRect(x,y,width-(2 * CHAR_WIDTH * textSize),height);
      buff[strlen(buff)-2]='\0';
      beginEditing();
    } 
  }
}


void editLimit::idle(void) {

  percentText::idle();
  if (!mEditing) {
    if (checkState()) {
      setTheLook();
    }
  }
}


// *****************************************************
//                    editWTime 
// *****************************************************


editWTime::editWTime(int x,int y,int width,int height,keyboard* inKeyboard)
  : waterTimeText(x,y,width,height) {
  
  mKeyboard = inKeyboard;
  setEventSet(fullClick);
}

  
editWTime::~editWTime(void) {  }


void editWTime::endEditing(void) {

  if (ourComPort.getOnline() && mSuccess) {
    ourComPort.setWTimeReg(atoi(buff));
  }
  mKeyboard->setEditField(NULL);            // Release the keyboard.
  setRect(x,y,width+(2 * CHAR_WIDTH * textSize),height);
  setColors(&white,&black);
  waterTimeText::endEditing();
  setTheLook();
}

  
void editWTime::doAction(event* inEvent,point* locaPt) {

  
  if (mEditing) {
    ourOS.beep();
    waterTimeText::doAction(inEvent,locaPt);
  } else {
    if (ourComPort.getOnline() && mKeyboard->mEditObj==NULL) {
      ourPanel->msg("Enter watering time in seconds.");
      ourOS.beep();
      screen->fillRect(this,&black);
      editLabel::setValue(ourComPort.getWaterTime());
      mKeyboard->setEditField(this);
      setColors(&yellow,&black);
      setRect(x,y,width-(2 * CHAR_WIDTH * textSize),height);
      buff[strlen(buff)-2]='\0';
      beginEditing();
    } 
  }
}


void editWTime::idle(void) {

  waterTimeText::idle();
  if (!mEditing) {
    if (checkState()) {
      setTheLook();
    }
  }
}


// *****************************************************
//                    editSTime 
// *****************************************************


editSTime::editSTime(int x,int y,int width,int height,keyboard* inKeyboard)
  : soakTimeText(x,y,width,height) {
  
  mKeyboard = inKeyboard;
  setEventSet(fullClick);
}

  
editSTime::~editSTime(void) {  }


void editSTime::endEditing(void) {

  if (ourComPort.getOnline() && mSuccess) {
    ourComPort.setSTimeReg(atoi(buff));
  }
  mKeyboard->setEditField(NULL);            // Release the keyboard.
  setRect(x,y,width+(2 * CHAR_WIDTH * textSize),height);
  setColors(&white,&black);
  soakTimeText::endEditing();
  setTheLook();
}


void editSTime::doAction(event* inEvent,point* locaPt) {

  if (mEditing) {
    ourOS.beep();
    soakTimeText::doAction(inEvent,locaPt);
  } else {
    if (ourComPort.getOnline() && mKeyboard->mEditObj==NULL) {
      ourPanel->msg("Enter soaking time in seconds.");
      ourOS.beep();
      screen->fillRect(this,&black);
      mKeyboard->setEditField(this);
      setColors(&yellow,&black);
      setRect(x,y,width-(2 * CHAR_WIDTH * textSize),height);
      buff[strlen(buff)-2]='\0';
      beginEditing();
    } 
  }
}


void editSTime::idle(void) {

  soakTimeText::idle();
  if (!mEditing) {
    if (checkState()) {
      setTheLook();
    }
  }
}
