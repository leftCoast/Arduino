#include "homePanel.h"
#include "floraOS.h"


// *****************************************************
// ******************   homeScreen   *******************
// *****************************************************

#define LABEL_W       138
#define LABEL_H       18
#define LINE_SPACE    20

#define CURR_MOIST_BMP  "/images/currM.bmp"
#define CURR_TEMP_BMP   "/images/currT.bmp"
#define TOTAL_WATER_BMP "/images/totalW.bmp"
#define TOTAL_TIME_BMP  "/images/totalT.bmp"
#define LIMIT_BMP       "/images/limit.bmp"
#define WATER_TIME_BMP  "/images/wTime.bmp"
#define SOAK_TIME_BMP   "/images/sTime.bmp"

#define CONTROL_PANEL_BMP "/icons/H2OCan32.bmp"
#define DATA_PANEL_BMP    "/icons/graph32.bmp"


homeScreen::homeScreen(void) { }

homeScreen::~homeScreen(void) { }
  
void homeScreen::setup(void) {

  int traceY;
  int stepY;
  
  traceY = 30;
  stepY  = LINE_SPACE;

  mNameLabel = new fontLabel(10,traceY,220,18);
  mNameLabel->setColors(&white,&black);
  mNameLabel->setJustify(TEXT_CENTER);
  mNameLabel->setValue("Plant bot, dummy");
  addObj(mNameLabel);

  traceY = traceY + stepY;
  
  traceY = traceY + stepY;  
  bmpObj* currM = new bmpObj(0,traceY,LABEL_W,LABEL_H,CURR_MOIST_BMP);
  currM->begin();
  addObj(currM);
  
  traceY = traceY + stepY;
  bmpObj* currT = new bmpObj(0,traceY,LABEL_W,LABEL_H,CURR_TEMP_BMP);
  currT->begin();
  addObj(currT);

  traceY = traceY + stepY;
  bmpObj* totalW = new bmpObj(0,traceY,LABEL_W,LABEL_H,TOTAL_WATER_BMP);
  totalW->begin();
  addObj(totalW);

  traceY = traceY + stepY;
  bmpObj* totalT = new bmpObj(0,traceY,LABEL_W,LABEL_H,TOTAL_TIME_BMP);
  totalT->begin();
  addObj(totalT);

  traceY = traceY + 2 * stepY;
  bmpObj* limit = new bmpObj(0,traceY,LABEL_W,LABEL_H,LIMIT_BMP);
  limit->begin();
  addObj(limit);

  traceY = traceY + stepY;
  bmpObj* waterT = new bmpObj(0,traceY,LABEL_W,LABEL_H,WATER_TIME_BMP);
  waterT->begin();
  addObj(waterT);

  traceY = traceY + stepY;
  bmpObj* soakT = new bmpObj(0,traceY,LABEL_W,LABEL_H,SOAK_TIME_BMP);
  soakT->begin();
  addObj(soakT);

  traceY = 285;
  appIcon*  waterCan = new appIcon(70,traceY,controlApp,CONTROL_PANEL_BMP);
  waterCan->begin();
  addObj(waterCan);

  appIcon*  dataLog = new appIcon(138,traceY,controlApp,DATA_PANEL_BMP);
  dataLog->begin();
  addObj(dataLog);
}


void homeScreen::loop(void) { }


void homeScreen::drawSelf(void) { screen->fillScreen(&black); }
