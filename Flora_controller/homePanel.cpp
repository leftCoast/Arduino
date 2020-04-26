#include "homePanel.h"
#include "floraOS.h"
#include "tools.h"
//#include "debug.h"


// *****************************************************
//                        waterBtn
// *****************************************************


waterBtn::waterBtn(int x, int y,int width, int height)
  :bmpObj(x,y,width,height),
  onlineIntStateTracker() {
    
  setEventSet(touchLift);
  setTheLook();
  hookup();
}

  
waterBtn::~waterBtn(void) {  }


void waterBtn::setTheLook(void) {
  
  if (mCurrentState.value && mCurrentState.online) {
    openPipe(WATER_OFF_BMP);
  } else {
    openPipe(WATER_ON_BMP);
  }
  needRefresh = true;
}


// Reading current state..
void  waterBtn::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getPump();
}


void  waterBtn::doAction(void) {
  ourOS.beep();
  screen->fillRect((rect*)this,&white);                 // Give it a flash.
  if (ourComPort.setWaterReg(!(mCurrentState.value))) {
    mCurrentState.value = !(mCurrentState.value);       // Looks like it worked.
    setTheLook();                                       // Show it to the user.
  }
}


void  waterBtn::idle(void) { if (checkState()) setTheLook(); }


// *****************************************************
//                      homeScreen
// *****************************************************

#define TOP_Y         20
#define LABEL_W       138
#define LABEL_H       18
#define LINE_SPACE    20

#define LIVE_X        LABEL_W+5
#define LIVE_W        90

#define CONTROL_PANEL_BMP "/system/icons/standard/edit32.bmp"
#define DATA_PANEL_BMP    "/system/icons/plantBot/graph32.bmp"
#define CALC_PANEL_BMP    "/system/icons/RPNCalc/calc32.bmp"
#define STERM_PANEL_BMP   "/system/icons/sTerm/term32.bmp"

homeScreen::homeScreen(void) { }

homeScreen::~homeScreen(void) { }
  
void homeScreen::setup(void) {

  int traceY;
  int stepY;
  
  traceY = TOP_Y;
  stepY  = LINE_SPACE;

  nameText* mNameLabel = new nameText(10,traceY,230,18);
  addObj(mNameLabel);

  traceY = traceY + 1.5*stepY;
  stateText*  stText = new stateText(10,traceY,230,18);
  addObj(stText);
  
  traceY = traceY + 2 * stepY;  
  bmpObj* currM = new bmpObj(0,traceY,LABEL_W,LABEL_H,CURR_MOIST_BMP);
  currM->begin();
  addObj(currM);

  currentMoistureText* mText = new currentMoistureText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(mText);

  /*
  traceY = traceY + stepY;
  bmpObj* currT = new bmpObj(0,traceY,LABEL_W,LABEL_H,CURR_TEMP_BMP);
  currT->begin();
  addObj(currT);

  currentTempText* tText = new currentTempText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(tText);
  */
  
  traceY = traceY + stepY;
  bmpObj* totalW = new bmpObj(0,traceY,LABEL_W,LABEL_H,TOTAL_WATER_BMP);
  totalW->begin();
  addObj(totalW);

  totalWaterText*  twText = new totalWaterText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(twText);
  
  traceY = traceY + stepY;
  bmpObj* totalT = new bmpObj(0,traceY,LABEL_W,LABEL_H,TOTAL_TIME_BMP);
  totalT->begin();
  addObj(totalT);

  totalLogTimeText*  ttText = new totalLogTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(ttText);
  
  traceY = traceY + 2 * stepY;
  bmpObj* limit = new bmpObj(0,traceY,LABEL_W,LABEL_H,LIMIT_BMP);
  limit->begin();
  addObj(limit);

  moistureLimitText*  lText = new moistureLimitText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(lText);
  
  traceY = traceY + stepY;
  bmpObj* waterT = new bmpObj(0,traceY,LABEL_W,LABEL_H,WATER_TIME_BMP);
  waterT->begin();
  addObj(waterT);

  waterTimeText*  wtText = new waterTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(wtText);
  
  traceY = traceY + stepY;
  bmpObj* soakT = new bmpObj(0,traceY,LABEL_W,LABEL_H,SOAK_TIME_BMP);
  soakT->begin();
  addObj(soakT);

  soakTimeText*  sText = new soakTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(sText);
  
  traceY = 285;

  int  traceX = 0;
  int  stepX = 52;
  
  mWaterBtn = new waterBtn(traceX,traceY,32,32);
  mWaterBtn->begin();
  addObj(mWaterBtn);

  
  traceX = traceX + stepX;
  appIcon*  editbtn = new appIcon(traceX,traceY,controlApp,CONTROL_PANEL_BMP);
  editbtn->begin();
  addObj(editbtn);

  traceX = traceX + stepX;
  appIcon*  dataLog = new appIcon(traceX,traceY,loggingApp,DATA_PANEL_BMP);
  dataLog->begin();
  addObj(dataLog);

  traceX = traceX + stepX;
  appIcon*  term = new appIcon(traceX,traceY,sTermApp,STERM_PANEL_BMP);
  term->begin();
  addObj(term);

  traceX = traceX + stepX;
  appIcon*  calc = new appIcon(traceX,traceY,calcApp,CALC_PANEL_BMP);
  calc->begin();
  addObj(calc);
}


void homeScreen::loop(void) { }


void homeScreen::drawSelf(void) { screen->fillScreen(&black); } 
