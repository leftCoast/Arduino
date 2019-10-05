#include "loggingPanel.h"
#include <IOandKeys.h>
#include <bmpObj.h>
#include "floraOS.h"


#define EB_X    74
#define EB_Y    162
#define EB_W    240-74
#define EB_H    22

#define ET_INSET  2
#define ET_X      EB_X+ET_INSET
#define ET_Y      EB_Y+4
#define ET_W      EB_W-(2*ET_INSET)
#define ET_H      10

#define LABEL_W       138
#define LABEL_H       18
#define SHORT_LABEL_W 72

#define GREEN_ON_32_BMP   "/icons/grnOn32.bmp"
#define GREEN_OFF_32_BMP  "/icons/grnOff32.bmp"

loggingPanel* logPanel;

// *****************************************************
//                        enableLogBtn
// *****************************************************


enableLogBtn::enableLogBtn(int x, int y,int width, int height)
  :bmpFlasher(x,y,width,height,GREEN_ON_32_BMP,GREEN_OFF_32_BMP),
  onlineIntStateTracker() {
    
  setEventSet(touchLift);
  hookup();
}

  
enableLogBtn::~enableLogBtn(void) {  }


void enableLogBtn::setTheLook(void) {

  if (mCurrentState.value && mCurrentState.online) {
    setBlink(true);
  } else {
     setBlink(false);
  }
}


// Reading current state..
void  enableLogBtn::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getLogging();
}


void  enableLogBtn::doAction(void) {
  
  ourOS.beep();
  screen->fillRect((rect*)this,&white);                 // Give it a flash.
  if (ourComPort.setLoggingReg(!(mCurrentState.value))) {
    mCurrentState.value = !(mCurrentState.value);       // Looks like it worked.
    setTheLook();                                       // Show it to the user.
  }
}


void  enableLogBtn::idle(void) { 
  
  bmpFlasher::idle();
  if (checkState()) {
    setTheLook(); 
  }
}



// *****************************************************
//                   deleteLogBtn
// *****************************************************


deleteLogBtn::deleteLogBtn(int x, int y)
  : iconButton(x,y,CUT32_BMP) {  }

  
deleteLogBtn::~deleteLogBtn(void) {  }
    
void deleteLogBtn::doAction(void) {
  
  ourOS.beep();
  if (ourComPort.clearLogCom()) {
    logPanel->msg("Logfile delted.");
  } else {
    logPanel->msg("Unable to delete logfile.");
  }
}



// *****************************************************
//                   copyLogBtn
// *****************************************************


copyLogBtn::copyLogBtn(int x, int y,ourKeyboard* akeyboard,panel* ourPanel)
  : iconButton(x,y,COPY32_BMP) {
  
  mPanel      = ourPanel;
  mEditLabel  = NULL;
  mKeyboard   = akeyboard;
  if (ourComPort.logCopyComActive()) {
    mXferState = xferRunning;
  } else {
    mXferState = xferIdle;
  }
  uiTimer.setTime(1000);
  uiTimer.start();
  hookup();
}

  
copyLogBtn::~copyLogBtn(void) { deleteEditor(); }
    

void copyLogBtn::startFileTransfer(void) {

  logPanel->msg("Enter filename, click Ok to start.");
  if (mEditLabel) {
    mKeyboard->setEditField(NULL);
    delete(mEditLabel);
    mEditLabel = NULL;
  }
  mEditLabel = new editLabel(ET_X+2,ET_Y+6,ET_W,ET_H,"",1);
  mEditLabel->setColors(&yellow,&black);
  mEditLabel->setJustify(TEXT_LEFT);
  mEditLabel->setEventSet(touchLift);
  mEditLabel->setValue("logfile.txt");
  mPanel->addObj(mEditLabel);
  mPanel->setNeedRefresh(false);
  mKeyboard->setEditField(mEditLabel);
  mEditLabel->beginEditing();
  mXferState = xferStarted;
}


void copyLogBtn::deleteEditor() {

  if (mEditLabel) { 
    screen->fillRect(mEditLabel,&black);
    delete(mEditLabel);
    mEditLabel = NULL;
    mKeyboard->setEditField(NULL);
  }
}


void copyLogBtn::doAction(void) {
  
  ourOS.beep();
  if (mXferState == xferIdle && ourComPort.getOnline() && !ourComPort.logCopyComActive()) {
    startFileTransfer(); 
  } else {
    logPanel->msg("Can't start for some reason.");
  }
}


void copyLogBtn::idle() {

  char  strBuff[30];
  int   percent;

  switch (mXferState) {
    case xferIdle       : break;
    case xferStarted    : 
      if (!mEditLabel->mEditing) {
        if (mEditLabel->mSuccess) {
          if (ourComPort.startLogCopyCom(mEditLabel->buff)) {
            logPanel->msg("Transfer started.");
            mXferState = xferRunning;
          } else {
            logPanel->msg("Transfer failed to start.");
            mXferState = xferIdle;
          }
          deleteEditor();
        } else {
          logPanel->msg("Transfer canceled.");
          deleteEditor();
          mXferState = xferIdle;
        }
      }
    break;
    case xferRunning  :
      if (uiTimer.ding()) {
        uiTimer.start();
        if (ourComPort.logCopyComActive()) {
          percent = ourComPort.logCopyPercent();
          if (percent<10) {                                             // If its 1-9..
            snprintf(strBuff,30,"Transfer progress :   %d %%",percent);  // put in two spaces.
          } else if (percent<100) {                                     // Else if its less than 100..               
            snprintf(strBuff,30,"Transfer progress :  %d %%",percent);   // One space.
          } else {                                                      // Else.. (>99) 
            snprintf(strBuff,30,"Transfer progress : %d %%",percent);    // No spaces.
          }
          logPanel->msg(strBuff);
        } else {
          logPanel->msg("Transfer complete.");
          deleteEditor();
          mXferState = xferIdle;
        }
      }
    break;
  }
}



// *****************************************************
//                      loggingPanel
// *****************************************************


loggingPanel::loggingPanel(void)
  : panel(loggingApp) { }


loggingPanel::~loggingPanel(void) { }

          
void loggingPanel::setup(void) {

  int traceY;
  int stepY;
  
  traceY = 40;
  stepY = 40;
  
  logPanel = this;
  
  mMessage = new liveText(30,9,210,10,100);
  mMessage->setColors(&yellow,&black);
  mMessage->setJustify(TEXT_LEFT);
  mMessage->addAColor(0,&yellow);
  mMessage->addAColor(3000,&yellow);
  mMessage->addAColor(4000,&black);
  mMessage->hold();
  addObj(mMessage);

  // LOGGING ON / OFF BUTTON
  enableLogBtn* loggingButton = new enableLogBtn(154,traceY,32,32);
  loggingButton->setTimes(500,1000);  
  loggingButton->setBlink(false);
  addObj(loggingButton);

  // LOGGING ON / OFF LABEL
  bmpObj* enableLbl = new bmpObj(0,traceY+7,LABEL_W,LABEL_H,ENABLE_LOG_BMP);
  enableLbl->begin();
  addObj(enableLbl);

  // KEYBOARD
  ourKeyboard*  theKeybaord = new ourKeyboard(NULL,true);
  theKeybaord->loadKeys();
  addObj(theKeybaord);

  // COPY LOGFILE BUTTON
  traceY = traceY + stepY;
  copyLogBtn* copyLogfile = new copyLogBtn(154,traceY,theKeybaord,this);
  copyLogfile->begin();
  addObj(copyLogfile);

  // COPY LOGFILE LABEL
  bmpObj* copyLbl = new bmpObj(0,traceY+7,LABEL_W,LABEL_H,COPY_LOG_BMP);
  copyLbl->begin();
  addObj(copyLbl);

  // DELETE LOGFILE BUTTON
  traceY = traceY + stepY;
  deleteLogBtn* deleteLogfile = new deleteLogBtn(154,traceY);
  deleteLogfile->begin();
  addObj(deleteLogfile);

  // DELETE LOGFILE LABEL
  bmpObj* cutLbl = new bmpObj(0,traceY+7,LABEL_W,LABEL_H,DELETE_LOG_BMP);
  cutLbl->begin();
  addObj(cutLbl);

  // LOGFILE NAME LABEL
  bmpObj* fNameLbl = new bmpObj(0,ET_Y,SHORT_LABEL_W,LABEL_H,FILE_NAME_BMP);
  fNameLbl->begin();
  addObj(fNameLbl);

}


void loggingPanel::msg(char* text) {

  mMessage->setValue(text);
  mMessage->release();
}


void loggingPanel::drawSelf(void) { screen->fillScreen(&black); }
