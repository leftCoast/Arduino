#include "homePanel.h"
#include "switchable.h"
#include "floraOS.h"
#include "tools.h"
#include "debug.h"

#define SEC_IN_HOUR 3600
#define SEC_IN_DAY  86400
#define SEC_IN_WEEK 604800

char  tTstring[100];

void timeFormatter(unsigned long sec) {
  
  int   weeks;
  int   days;
  int   hours;
  int   minutes;
  char* strPtr;
  float fract;
  int   dec;
  
  if (sec==0) {
    strcpy(tTstring,"0.0 s");
    return;
  }
  if (sec>=SEC_IN_WEEK) {
    weeks = sec/SEC_IN_WEEK;
    sec = sec-weeks*SEC_IN_WEEK;
    fract = sec/(float)SEC_IN_WEEK;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d w",weeks,dec);
  } else if (sec>=SEC_IN_DAY) {
    days = sec/SEC_IN_DAY;
    sec = sec-days*SEC_IN_DAY;
    fract = sec/(float)SEC_IN_DAY;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d d",days,dec);
  }  else if (sec>=SEC_IN_HOUR) {
    hours = sec/SEC_IN_HOUR;
    sec = sec-hours*SEC_IN_HOUR;
    fract = sec/(float)SEC_IN_HOUR;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d h", hours,dec);
  }  else if (sec>=60) {
    minutes = sec/60;
    sec = sec-minutes*60;
    fract = sec/60;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d m",minutes,dec);
  } else {
    snprintf (tTstring,5,"%d s",sec);
  }
}



// *****************************************************
//                     nameText
// *****************************************************


nameText::nameText(int x, int y,int width, int height)
  : onlineFText(x,y,width,height) {

  setTextSize(1);
  setJustify(TEXT_CENTER);
}

  
nameText::~nameText(void) {  }


void nameText::setValue(void) {

  if (mOnline) {
    onlineFText::setValue(ourComPort.getName());
  } else {
    onlineFText::setValue("Offline");
  }
}


void nameText::setState(void) { setValue(); }


void nameText::idle() {

  onlineFText::idle();
  if (mOnline) {
    if (strcmp(buff,ourComPort.getName())) {
      setValue();
    }
  }
}



// *****************************************************
//                     sTimeText
// *****************************************************


sTimeText::sTimeText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { mSoakTime = 0; }

  
sTimeText::~sTimeText(void) {  }


void sTimeText::setValue(void) {

  if (mOnline) {
    timeFormatter(mSoakTime);
    onlineText::setValue(tTstring);
  } else {
    onlineText::setValue("--- s");
  }
}


void sTimeText::setState(void) {

  if (mOnline) { mSoakTime = ourComPort.getSoakTime(); }
  setValue();
}


void sTimeText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mSoakTime!=ourComPort.getSoakTime()) {
      mSoakTime = ourComPort.getSoakTime();
      setValue();
    }
  }
}


// *****************************************************
//                     wTimeText
// *****************************************************

wTimeText::wTimeText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { mWaterTime = 0; }

  
wTimeText::~wTimeText(void) {  }


void wTimeText::setValue(void) {

  if (mOnline) {
    timeFormatter(mWaterTime);
    onlineText::setValue(tTstring);
  } else {
    onlineText::setValue("--- s");
  }
}


void wTimeText::setState(void) {

  if (mOnline) { mWaterTime = ourComPort.getWaterTime(); }
  setValue();
}


void wTimeText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mWaterTime!=ourComPort.getWaterTime()) {
      mWaterTime = ourComPort.getWaterTime();
      setValue();
    }
  }
}



// *****************************************************
//                     limitText
// *****************************************************


limitText::limitText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { mLimit = 0; }

  
limitText::~limitText(void) {  }


void limitText::setValue(void) {

  char  limit[7];
  
    if (mOnline) {
      snprintf (limit,5,"%d",mLimit);
      strcat(limit," %");
    } else {
      strcpy(limit,"--- %");
    }
    onlineText::setValue(limit);
}


void limitText::setState(void) {

  if (mOnline) { mLimit = ourComPort.getLimit(); }
  setValue();
}


void limitText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mLimit!=ourComPort.getLimit()) {
      mLimit = ourComPort.getLimit();
      setValue();
    }
  }
}



// *****************************************************
//                   totalWaterText
// *****************************************************


totalWaterText::totalWaterText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) {

  lPerSec = .01;
  setPrecision(1);
}

  
totalWaterText::~totalWaterText(void) {  }


void totalWaterText::setValue(void) {

  float res;
  char  wStr[10];
  int   prec;
  
  if (mOnline) {
    res = (mLogWetLines/2.0)*lPerSec;
    if (res>=100) {
      prec = 0;
    } else {
      prec = 1;
    }
    dtostrf(res,0,prec,wStr);
    strcat(wStr," l");
    onlineText::setValue(wStr);
  } else {
    onlineText::setValue("--- l");
  }
}


void totalWaterText::setState(void) {

  if (mOnline) { mLogWetLines = ourComPort.getLogWLines(); }
  setValue();
}


void totalWaterText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mLogWetLines!=ourComPort.getLogWLines()) {
      mLogWetLines = ourComPort.getLogWLines();
      setValue();
    }
  }
}



// *****************************************************
//                   totalTimeText
// *****************************************************


totalTimeText::totalTimeText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { }

  
totalTimeText::~totalTimeText(void) {  }


void totalTimeText::setValue(void) {

  if (mOnline) {
    timeFormatter(mLogLines/2);
    onlineText::setValue(tTstring);
  } else {
    onlineText::setValue("--- s");
  }
}


void totalTimeText::setState(void) {

  if (mOnline) { mLogLines = ourComPort.getLogLines(); }
  setValue();
}


void totalTimeText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mLogLines!=ourComPort.getLogLines()) {
      mLogLines = ourComPort.getLogLines();
      setValue();
    }
  }
}


// *****************************************************
//                     moistureText
// *****************************************************


moistureText::moistureText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { }

  
moistureText::~moistureText(void) {  }


void moistureText::setValue(void) {

  char  moistText[7];
  
    if (mOnline) {
      snprintf (moistText,5,"%d",mMoisture);
      strcat(moistText," %");
    } else {
      strcpy(moistText,"--- %");
    }
    onlineText::setValue(moistText);
}


void moistureText::setState(void) {

  if (mOnline) { mMoisture = ourComPort.getMoisture(); }
  setValue();
}


void moistureText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mMoisture!=ourComPort.getMoisture()) {
      mMoisture = ourComPort.getMoisture();
      setValue();
    }
  }
}



// *****************************************************
//                     tempText
// *****************************************************


tempText::tempText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { }

  
tempText::~tempText(void) {  }


void tempText::setValue(void) {

  char  aStr[7];
  
    if (mOnline) {
      snprintf (aStr,5,"%d",mTemp);
      strcat(aStr," C");
    } else {
      strcpy(aStr,"--- C");
    }
    onlineText::setValue(aStr);
}


void tempText::setState(void) {

  if (mOnline) { mTemp = ourComPort.getTemp(); }
  setValue();
}


void tempText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mTemp!=ourComPort.getTemp()) {
      mTemp = ourComPort.getTemp();
      setValue();
    }
  }
}


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


#define LABEL_W       138
#define LABEL_H       18
#define LINE_SPACE    20

#define LIVE_X        LABEL_W+5
#define LIVE_W        90


#define CURR_MOIST_BMP  "/images/currM.bmp"
#define CURR_TEMP_BMP   "/images/currT.bmp"
#define TOTAL_WATER_BMP "/images/totalW.bmp"
#define TOTAL_TIME_BMP  "/images/totalT.bmp"
#define LIMIT_BMP       "/images/limit.bmp"
#define WATER_TIME_BMP  "/images/wTime.bmp"
#define SOAK_TIME_BMP   "/images/sTime.bmp"

#define CONTROL_PANEL_BMP "/icons/edit32.bmp"
#define DATA_PANEL_BMP    "/icons/graph32.bmp"


homeScreen::homeScreen(void) { }

homeScreen::~homeScreen(void) { }
  
void homeScreen::setup(void) {

  int traceY;
  int stepY;
  
  traceY = 30;
  stepY  = LINE_SPACE;

  nameText* mNameLabel = new nameText(10,traceY,220,18);
  addObj(mNameLabel);

  traceY = traceY + stepY;
  
  traceY = traceY + stepY;  
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

  mWaterBtn = new waterBtn(44,traceY,32,32);
  mWaterBtn->select(true);
  addObj(mWaterBtn);
 
  appIcon*  editbtn = new appIcon(104,traceY,controlApp,CONTROL_PANEL_BMP);
  editbtn->begin();
  addObj(editbtn);


  appIcon*  dataLog = new appIcon(164,traceY,controlApp,DATA_PANEL_BMP);
  dataLog->begin();
  addObj(dataLog);
}


void homeScreen::loop(void) { }


void homeScreen::drawSelf(void) { screen->fillScreen(&black); }
