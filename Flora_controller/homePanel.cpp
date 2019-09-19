#include "homePanel.h"
#include "switchable.h"
#include "floraOS.h"
#include "tools.h"
#include "debug.h"


// *****************************************************
//                        waterBtn
// *****************************************************


waterBtn::waterBtn(int x, int y,int width, int height)
  :baseIconButton(x,y,width,height,"/icons/H2OOn32.bmp") { 
    
  mOnOff = true;  // True means we will turn it on.
  setTheLook();
}

  
waterBtn::~waterBtn(void) {  }


void waterBtn::setTheLook(void) {
  
  if (mOnOff) {
    openPipe(WATER_ON_BMP);
  } else {
    openPipe(WATER_OFF_BMP);
  }
}


void  waterBtn::doAction(event* inEvent,point* locaPt) {

  byte  outByte;

  
  if (inEvent->mType==touchEvent) {
    screen->fillRect((rect*)this,&white); // Give it a flash.
    if (ourComPort.setWaterReg(mOnOff)) {
      mOnOff = !mOnOff;
    }
    setTheLook();
  }
}



// *****************************************************
//                      homeScreen
// *****************************************************

#define TOP_Y         20
#define LABEL_W       138
#define LABEL_H       18
#define LINE_SPACE    20

#define LIVE_X        LABEL_W+5
#define LIVE_W        90

#define CONTROL_PANEL_BMP "/icons/edit32.bmp"
#define DATA_PANEL_BMP    "/icons/graph32.bmp"
#define CALC_PANEL_BMP    "/icons/calc32.bmp"


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

  moistureText* mText = new moistureText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(mText);
  
  traceY = traceY + stepY;
  bmpObj* currT = new bmpObj(0,traceY,LABEL_W,LABEL_H,CURR_TEMP_BMP);
  currT->begin();
  addObj(currT);

  tempText* tText = new tempText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(tText);
  
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

  totalTimeText*  ttText = new totalTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(ttText);
  
  traceY = traceY + 2 * stepY;
  bmpObj* limit = new bmpObj(0,traceY,LABEL_W,LABEL_H,LIMIT_BMP);
  limit->begin();
  addObj(limit);

  limitText*  lText = new limitText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(lText);
  
  traceY = traceY + stepY;
  bmpObj* waterT = new bmpObj(0,traceY,LABEL_W,LABEL_H,WATER_TIME_BMP);
  waterT->begin();
  addObj(waterT);

  wTimeText*  wtText = new wTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(wtText);
  
  traceY = traceY + stepY;
  bmpObj* soakT = new bmpObj(0,traceY,LABEL_W,LABEL_H,SOAK_TIME_BMP);
  soakT->begin();
  addObj(soakT);

  sTimeText*  sText = new sTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(sText);
  
  traceY = 285;

  int  traceX = 0;
  int  stepX = 60;
  
  mWaterBtn = new waterBtn(traceX,traceY,32,32);
  mWaterBtn->select(true);
  addObj(mWaterBtn);

  
  traceX = traceX + stepX;
  appIcon*  editbtn = new appIcon(traceX,traceY,controlApp,CONTROL_PANEL_BMP);
  editbtn->begin();
  addObj(editbtn);

  traceX = traceX + stepX;
  appIcon*  dataLog = new appIcon(traceX,traceY,controlApp,DATA_PANEL_BMP);
  dataLog->begin();
  addObj(dataLog);

  traceX = traceX + stepX;
  appIcon*  calc = new appIcon(traceX,traceY,calcApp,CALC_PANEL_BMP);
  calc->begin();
  addObj(calc);
}


void homeScreen::loop(void) { }


void homeScreen::drawSelf(void) { screen->fillScreen(&black); }
