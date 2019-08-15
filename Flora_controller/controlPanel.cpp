#include "controlPanel.h"


#define DRY_LIMIT     1
#define W_TIME        2
#define S_TIME        3

#define SELECT_X      162
#define SELECT_W      45
#define SELECT_H      20
#define SELECT_YOFFS  3

#define READ_TIME 500
#define MAX_WTIME 120000
#define MAX_STIME 600000

enum commandChoices { comIdle  ,comPumpOn, comPumpOff, comSetDryLimit, comSetWTime, comSetSTime };
commandChoices  commandChoice = comIdle;

char tempStrBuff[256];

// *****************************************************
// ******************    waterBtn   *********************
// *****************************************************


waterBtn::waterBtn(int x, int y,int width, int height)
  :baseButton("Water",x,y,width,height) { 
    
  mOnOff = true;
}

  
waterBtn::~waterBtn(void) {  }


void  waterBtn::doAction(event* inEvent,point* locaPt) {

  byte  outByte;
  
  if (inEvent->mType==touchEvent) {
    if (mOnOff) {
      outByte = pumpOn;
    } else {
      outByte = pumpOff;
    }
    ourComPort.sendBuff(&outByte,1,false);
    mOnOff = !mOnOff;
  }
}



// *****************************************************
// ******************   selectBtn   ********************
// *****************************************************


selectBtn::selectBtn(int x,int y,int width,int height,int inChoice,controlPanel* inPanel)
  :baseButton("",x,y,width,height) {
    
    mChoice = inChoice;
    mPanel = inPanel;
}

  
selectBtn::~selectBtn(void) {  }


int selectBtn::getChoice(void) { return mChoice; }


void selectBtn::drawSelf(void) {

  if (selectedVal==this) {
    screen->drawRoundRect(x, y, width, height, 4, &red);
  } else {
    screen->drawRoundRect(x, y, width, height, 4, &BGColor);
  }
}


void  selectBtn::doAction(event* inEvent,point* locaPt) {
  
  if (inEvent->mType==touchEvent) {
    if (selectedVal) {
      selectedVal->setNeedRefresh();
    }
    selectedVal = this;
    switch (mChoice) {
      case DRY_LIMIT  :
        mPanel->mSlider->setup(DEF_SLIDER_KNOB_W,DEF_SLIDER_LINE_H,0,100);
        mPanel->mSlider->setValue(mPanel->mLastLimit);
      break;
      case W_TIME     :
        mPanel->mSlider->setup(DEF_SLIDER_KNOB_W,DEF_SLIDER_LINE_H,0,MAX_WTIME/1000);
        mPanel->mSlider->setValue(mPanel->mLastWTime);
      break;
      case S_TIME     :
        mPanel->mSlider->setup(DEF_SLIDER_KNOB_W,DEF_SLIDER_LINE_H,0,MAX_STIME/1000);
        mPanel->mSlider->setValue(mPanel->mLastSTime);
      break;
    }
    mPanel->mSlider->select(true);
    //mPanel->mSliderLabel->select(true);
    mPanel->mOkBtn->select(true);
    mPanel->mCancelBtn->select(true);
    mPanel->mWaterBtn->select(false);
  }
}


selectBtn* selectedVal = NULL;



// *****************************************************
// *****************  editSlider   *********************
// *****************************************************


editSlider::editSlider(int x,int y,int width,int height) 
  : slider(x,y,width,height) { mLabel = NULL; }


editSlider::~editSlider(void) { }


void editSlider::setLabel(label* inLabel) { mLabel = inLabel; } 


void editSlider::setText(void) {

  int value;
  
  if (mLabel) {
      value = round(getValue());
      mLabel->setValue(value);
  }
}


void editSlider::doAction(event* inEvent,point* locaPt) {

  slider::doAction(inEvent,locaPt);
  if (inEvent->mType==dragOn||inEvent->mType==liftEvent) {
    setText();
  }
}



// *****************************************************
// ******************    okBtn    **********************
// *****************************************************


okBtn::okBtn(int x, int y,int width, int height,controlPanel* inPanel)
  : baseButton("Ok",x,y,width,height) {
    
  mPanel = inPanel;
  activeBColor.setColor(&green);
}

   
okBtn::~okBtn(void) {  }


void okBtn::doAction(event* inEvent,point* locaPt) {

  if (inEvent->mType==liftEvent) {
    mPanel->mSlider->select(false);
    mPanel->mCancelBtn->select(false);
    select(false);
    //mPanel->mSliderLabel->select(true);
    mPanel->mWaterBtn->select(true);
    if (selectedVal) {
      switch (selectedVal->getChoice()) {
        case DRY_LIMIT  : commandChoice = comSetDryLimit; break;
        case W_TIME     : commandChoice = comSetWTime;    break;
        case S_TIME     : commandChoice = comSetSTime;    break;
      }
      selectedVal->setNeedRefresh();
      selectedVal = NULL;
    }
  }
}



// *****************************************************
// ****************    cancelBtn    ********************
// *****************************************************


cancelBtn::cancelBtn(int x, int y,int width, int height,controlPanel* inPanel)
  : baseButton("Cancel",x,y,width,height) {

  mPanel = inPanel;
  activeBColor.setColor(&red);
}

   
cancelBtn::~cancelBtn(void) {  }


void cancelBtn::doAction(event* inEvent,point* locaPt) {

  if (inEvent->mType==liftEvent) {
    mPanel->mSlider->select(false);
    mPanel->mOkBtn->select(false);
    select(false);
    //mPanel->mSliderLabel->select(true);
    mPanel->mWaterBtn->select(true);
    if (selectedVal) selectedVal->setNeedRefresh();
    selectedVal = NULL;
  }
}


     
// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************


#define LINE_SPACE  20

enum dataSelect { dataStart, dataName, dataMoisture, dataDryLimit, dataWaterTime, dataSoakTime, dataEnd };
dataSelect  dataChoice = dataStart;


controlPanel::controlPanel(void) : panel(controlApp) { }


controlPanel::~controlPanel(void) { }


void controlPanel::setup(void) {

    int       traceY;
    colorObj  labelColor;

    labelColor.setColor(LC_LIGHT_BLUE);
    labelColor.blend(&blue,20);
    mReadTimer.setTime(READ_TIME);
    ourComPort.setReplyTimeout(50);

    traceY = 30;
    
    mNameLabel = new label(10,traceY,220,14,"...",2);
    mNameLabel->setColors(&labelColor,&black);
    addObj(mNameLabel);
  
    moistureText(0);
    traceY = traceY + LINE_SPACE;
    mMoistureLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mMoistureLabel->setColors(&labelColor,&black);
    addObj(mMoistureLabel);

    dryLimitText(0);
    traceY = traceY + (2*LINE_SPACE);
    mDryLimitLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mDryLimitLabel->setColors(&labelColor,&black);
    addObj(mDryLimitLabel);

    mDryLimitBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,DRY_LIMIT,this);
    addObj(mDryLimitBtn);
      
    waterTimeText(0);
    traceY = traceY + LINE_SPACE;
    mWaterTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mWaterTimeLabel->setColors(&labelColor,&black);
    addObj(mWaterTimeLabel);

    mWaterTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,W_TIME,this);
    addObj(mWaterTimeBtn);
    
    soakTimeText(0);
    traceY = traceY + LINE_SPACE;
    mSoakTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mSoakTimeLabel->setColors(&labelColor,&black);
    addObj(mSoakTimeLabel);

    mSoakTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,S_TIME,this);
    addObj(mSoakTimeBtn);

    traceY = traceY + 2*LINE_SPACE;
    mSliderLabel  = new label(120,traceY,100,14,"",1);
    mSliderLabel->setColors(&labelColor,&black);
    addObj(mSliderLabel);
    
    traceY = traceY + (LINE_SPACE);
    mSlider = new editSlider(20,traceY,200,20);
    mSlider->setLabel(mSliderLabel);
    mSlider->setValue(50);
    mSlider->select(false);
    addObj(mSlider);

    traceY = traceY + 2*LINE_SPACE;
    mOkBtn = new okBtn(90,traceY,35,20,this);
    mOkBtn->select(false);
    addObj(mOkBtn);

    mCancelBtn = new cancelBtn(150,traceY,80,20,this);
    mCancelBtn->select(false);
    addObj(mCancelBtn);
     
    traceY = traceY + 3*LINE_SPACE;
    mWaterBtn = new waterBtn(20,traceY,80,20);
    mWaterBtn->select(true);
    addObj(mWaterBtn);
}


void controlPanel::moistureText(int moisture) {

  char moistText[5];
  
  if (!moisture) {
    strcpy(moistText," 0");
  } else if (moisture<10) {
    snprintf (moistText,5," %d",moisture);
  } else {
    snprintf (moistText,5,"%d",moisture);
  }
  strcpy(tempStrBuff,"Moisture   :  ");
  strcat(tempStrBuff,moistText);
  strcat(tempStrBuff," %");
}


void controlPanel::dryLimitText(int dryLimit) {

  char dryLimitText[5];
  
  if (!dryLimit) {
    strcpy(dryLimitText,"  0");
  } else if (dryLimit<10) {
    snprintf (dryLimitText,5,"  %d",dryLimit);
  } else if (dryLimit<99) {
    snprintf (dryLimitText,5," %d",dryLimit);
  } else {
    snprintf (dryLimitText,5,"%d",dryLimit);
  }
  strcpy(tempStrBuff,"Dry limit  : ");
  strcat(tempStrBuff,dryLimitText);
  strcat(tempStrBuff," %");
}


void controlPanel::waterTimeText(unsigned long wTime) {

  char wText[20];

  wTime = round(wTime/1000.0);          // Comes in as ms, we want seconds.
  if (wTime<0) {
    wTime=0;
  }
  if (!wTime) {                         
    strcpy(wText,"---");
  } else if (wTime<10) {                // If its 1-9..
    snprintf (wText,5,"  %d",wTime);    // put in two spaces.
  } else if (wTime<100) {               // Else if its less than 100..               
    snprintf (wText,5," %d",wTime);     // One space.
  } else {                              // Else.. (>99) 
    snprintf (wText,5,"%d",wTime);      // No spaces.
  }
  strcpy(tempStrBuff,"Water time : ");
  strcat(tempStrBuff,wText);
  strcat(tempStrBuff," s");
}

 
void controlPanel::soakTimeText(unsigned long sTime) {

    char sText[20];

   sTime = round(sTime/1000.0);              // Comes in as ms, we want seconds.
  if (sTime<0) {
    sTime=0;
  }
  if (!sTime) {                         
    strcpy(sText,"---");
  } else if (sTime<10) {                // If its 1-9..
    snprintf (sText,5,"  %d",sTime);    // put in two spaces.
  } else if (sTime<100) {               // Else if its less than 100..               
    snprintf (sText,5," %d",sTime);     // One space.
  } else {                              // Else.. (>99) 
    snprintf (sText,5,"%d",sTime);      // No spaces.
  }
  strcpy(tempStrBuff,"Soak time  : ");
  strcat(tempStrBuff,sText);
  strcat(tempStrBuff," s");
}


void controlPanel::checkMoisture(void) {
  
  byte  byteBuff;
  
  byteBuff = readMoisture;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==1) {
    ourComPort.readBuff(&byteBuff);
    moistureText(byteBuff);
    mMoistureLabel->setValue(tempStrBuff);
    mMoistureLabel->setNeedRefresh();
  } else {
    ourComPort.dumpBuff();
  }
}


void controlPanel::checkDryLimit(void) {

  byte  byteBuff;
  
  byteBuff = readDryLimit;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==1) {
    ourComPort.readBuff(&byteBuff);
    mLastLimit = byteBuff;
    dryLimitText(byteBuff);
    mDryLimitLabel->setValue(tempStrBuff);
    mDryLimitLabel->setNeedRefresh();
  } else {
    ourComPort.dumpBuff();
  }
}


void controlPanel::checkWaterTime(void) {

  byte          byteBuff;
  unsigned long longBuff;
  
  byteBuff = readWaterTime;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==sizeof(unsigned long)) {
    ourComPort.readBuff((byte*)&longBuff);
    mLastWTime = longBuff/1000;
    waterTimeText(longBuff);
    mWaterTimeLabel->setValue(tempStrBuff);
    mWaterTimeLabel->setNeedRefresh();
  } else {
    ourComPort.dumpBuff();
  }
}


void controlPanel::checkSoakTime(void) {

  byte          byteBuff;
  unsigned long longBuff;
  
  byteBuff = readSoakTime;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==sizeof(unsigned long)) {
    ourComPort.readBuff((byte*)&longBuff);
    mLastSTime = longBuff/1000;
    soakTimeText(longBuff);
    mSoakTimeLabel->setValue(tempStrBuff);
    mSoakTimeLabel->setNeedRefresh();
  } else {
    ourComPort.dumpBuff();
  }
}


void controlPanel::checkName(void) { 

  byte  byteBuff;
  char  nameBuff[NAME_BUFF_BYTES];
  
  byteBuff = readName;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff&&byteBuff<=NAME_BUFF_BYTES) {
    ourComPort.readBuff((byte*)&nameBuff);
    mNameLabel->setValue(nameBuff);
    mSoakTimeLabel->setNeedRefresh();
  } else {
    ourComPort.dumpBuff();
  }
}


void controlPanel::doComPump(bool onOff) {

  byte  outByte;
  
  if (onOff) {
    outByte = pumpOn;
  } else {
    outByte = pumpOff;
  }
  ourComPort.sendBuff(&outByte,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  if (ourComPort.haveBuff()) {
    ourComPort.dumpBuff();
  }
  commandChoice = comIdle;
}


void controlPanel::doComSetDryLimit(float limit) {

  byte  byteBuff[2];
  byte  byteLimit;

  if (limit<0) byteLimit = 0;
  else if (limit>100) byteLimit = 100;
  else byteLimit = round(limit);
  byteBuff[0] = (byte)setDryLimit;
  byteBuff[1] = byteLimit;
  ourComPort.sendBuff(byteBuff,2,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  if (ourComPort.haveBuff()) {
    ourComPort.dumpBuff();
  }
}


void controlPanel::doComSetWTime(float wTime) { 

  byte  byteBuff[sizeof(unsigned long)+1];
  unsigned long   longTime;
  unsigned long*  longPtr;
  
  if (wTime<0) wTime = 0;
  else if (wTime>MAX_WTIME) wTime = MAX_WTIME;
  longTime = round(wTime*1000);
  byteBuff[0] = (byte)setWaterTime;
  longPtr = (unsigned long*)(&(byteBuff[1]));
  *longPtr = longTime;
  ourComPort.sendBuff(byteBuff,sizeof(unsigned long)+1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  if (ourComPort.haveBuff()) {
    ourComPort.dumpBuff();
  }
}


void controlPanel::doComSetSTime(float sTime) {

  byte  byteBuff[sizeof(unsigned long)+1];
  unsigned long   longTime;
  unsigned long*  longPtr;
  
  if (sTime<0) sTime = 0;
  else if (sTime>MAX_STIME) sTime = MAX_STIME;
  longTime = round(sTime*1000);
  byteBuff[0] = (byte)setSoakTime;
  longPtr = (unsigned long*)(&(byteBuff[1]));
  *longPtr = longTime;
  ourComPort.sendBuff(byteBuff,sizeof(unsigned long)+1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
  if (ourComPort.haveBuff()) {
    ourComPort.dumpBuff();
  }
}

          
void controlPanel::loop(void) {

  if (mReadTimer.ding()&&!selectedVal) {                        
    switch (dataChoice) {
      case dataStart      : break;
      case dataName       : checkName();
      case dataMoisture   : checkMoisture();
      case dataDryLimit   : checkDryLimit();
      case dataWaterTime  : checkWaterTime();
      case dataSoakTime   : checkSoakTime();
      case dataEnd        : dataChoice = dataStart; break;
    }
    dataChoice = (dataSelect)(dataChoice + 1);
    mReadTimer.start();
  }
  if (commandChoice!=comIdle) Serial.println(commandChoice);
  switch (commandChoice) {
    case comIdle        : break;
    case comPumpOn      : doComPump(true);  break;
    case comPumpOff     : doComPump(false); break;
    case comSetDryLimit : doComSetDryLimit(mSlider->getValue()); break;
    case comSetWTime    : doComSetWTime(mSlider->getValue()); break;
    case comSetSTime    : doComSetSTime(mSlider->getValue()); break;
  }
  commandChoice = comIdle;
}


void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}

  
void controlPanel::close(void) { }


void controlPanel::closing(void) { }
