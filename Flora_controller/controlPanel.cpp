#include "controlPanel.h"


#define DRY_LIMIT     1
#define W_TIME        2
#define S_TIME        3

#define SELECT_X      162
#define SELECT_W      45
#define SELECT_H      20
#define SELECT_YOFFS  3

#define READ_TIME     250
#define SLEEP_TIME    5
#define MAX_WTIME     120000
#define MAX_STIME     600000

enum commandChoices { comIdle  ,comPumpOn, comPumpOff, comSetDryLimit, comSetWTime, comSetSTime };
commandChoices  commandChoice = comIdle;

char tempStrBuff[256];



// *****************************************************
// ******************    waterBtn   *********************
// *****************************************************


waterBtn::waterBtn(int x, int y,int width, int height)
  :baseButton("Pump",x,y,width,height) { 
    
  mOnOff = true;  // True means we will turn it on.
  setTheLook();
}

  
waterBtn::~waterBtn(void) {  }



void    waterBtn::setTheLook(void) {

  if (mOnOff) {
    setText("Water");
    activeBColor.setColor(LC_BLUE);
  } else {
    setText("Off");
    activeBColor.setColor(LC_RED);
  }
}


void  waterBtn::doAction(event* inEvent,point* locaPt) {

  byte  outByte;
  
  if (inEvent->mType==touchEvent) {
    if (mOnOff) {
      commandChoice = comPumpOn;
    } else {
      commandChoice = comPumpOff;
    }
    mOnOff = !mOnOff;
    setTheLook();
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

void editSlider::select(bool onOff) {

  slider::select(onOff);
  if (!onOff) mLabel->setValue("");
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
    mPanel->mWaterBtn->select(true);
    if (selectedVal) selectedVal->setNeedRefresh();
    selectedVal = NULL;
  }
}


     
// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************


#define LINE_SPACE  20

enum dataSelect { dataStart, dataState, dataName, dataMoisture, dataDryLimit, dataWaterTime, dataSoakTime, dataEnd };
dataSelect  dataChoice = dataStart;


controlPanel::controlPanel(void) : panel(controlApp) {

  mLastLimit = 50;
  mLastWTime = 15;
  mLastSTime = 30;
}


controlPanel::~controlPanel(void) { }


void controlPanel::setup(void) {

    int       traceY;
    colorObj  labelColor;

    labelColor.setColor(LC_LIGHT_BLUE);
    labelColor.blend(&blue,20);
    mReadTimer.setTime(READ_TIME);

    traceY = 30;
    
    mNameLabel = new label(10,traceY,220,14,"...",2);
    mNameLabel->setColors(&labelColor,&black);
    addObj(mNameLabel);
    
    mNameLight = new colorRect(0,traceY+4,6,6);
    mNameLight->setColor(LC_RED);
    addObj(mNameLight);
    
    traceY = traceY + LINE_SPACE;
    stateText(-1);
    mStateLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mStateLabel->setColors(&labelColor,&black);
    addObj(mStateLabel);

    mStateLight = new colorRect(0,traceY+4,6,6);
    mStateLight->setColor(LC_RED);
    addObj(mStateLight);
    
    moistureText(0);
    traceY = traceY + LINE_SPACE;
    mMoistureLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mMoistureLabel->setColors(&labelColor,&black);
    addObj(mMoistureLabel);

    mMoistureLight = new colorRect(0,traceY+4,6,6);
    mMoistureLight->setColor(LC_RED);
    addObj(mMoistureLight);
    
    dryLimitText(0);
    traceY = traceY + (2*LINE_SPACE);
    mDryLimitLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mDryLimitLabel->setColors(&labelColor,&black);
    addObj(mDryLimitLabel);

    mDryLimitLight = new colorRect(0,traceY+4,6,6);
    mDryLimitLight->setColor(LC_RED);
    addObj(mDryLimitLight);
    
    mDryLimitBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,DRY_LIMIT,this);
    addObj(mDryLimitBtn);
      
    waterTimeText(0);
    traceY = traceY + LINE_SPACE;
    mWaterTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mWaterTimeLabel->setColors(&labelColor,&black);
    addObj(mWaterTimeLabel);

    mWaterTimeLight = new colorRect(0,traceY+4,6,6);
    mWaterTimeLight->setColor(LC_RED);
    addObj(mWaterTimeLight);
    
    mWaterTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,W_TIME,this);
    addObj(mWaterTimeBtn);
    
    soakTimeText(0);
    traceY = traceY + LINE_SPACE;
    mSoakTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mSoakTimeLabel->setColors(&labelColor,&black);
    addObj(mSoakTimeLabel);

    mSoakTimeLight = new colorRect(0,traceY+4,6,6);
    mSoakTimeLight->setColor(LC_RED);
    addObj(mSoakTimeLight);
    
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
     
    traceY = traceY + 2*LINE_SPACE;
    mWaterBtn = new waterBtn(20,traceY,80,20);
    mWaterBtn->select(true);
    addObj(mWaterBtn);
}


void controlPanel::stateText(int state) {

  if (state==-1) {
    strcpy(tempStrBuff,"Offline");
  } else {
    strcpy(tempStrBuff,"State : ");
    switch(state) {
      case 0  : strcat(tempStrBuff,"Watching "); break;
      case 1  :strcat(tempStrBuff, "Watering "); break;
      case 2  :strcat(tempStrBuff, "Soaking  "); break;
    }
  }
}


void controlPanel::moistureText(int moisture) {

  char moistText[5];
  
  if (!moisture) {
    strcpy(moistText,"---");
  } else if (moisture<10) {
    snprintf (moistText,5,"  %d",moisture);
  } else if (moisture<100) {
    snprintf (moistText,5," %d",moisture);
  } else {
    snprintf (moistText,5,"%d",moisture);
  }
  strcpy(tempStrBuff,"Moisture   : ");
  strcat(tempStrBuff,moistText);
  strcat(tempStrBuff," %");
}


void controlPanel::dryLimitText(int dryLimit) {

  char dryLimitText[5];
  
  if (dryLimit<0) {
    strcpy(dryLimitText,"---");
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


void controlPanel::waterTimeText(long wTime) {

  char wText[20];

  
  if (wTime<0) {                         
    strcpy(wText,"---");
  } else {
    wTime = round(wTime/1000.0);          // Comes in as ms, we want seconds.
    if (wTime<10) {                       // If its 1-9..
      snprintf (wText,5,"  %d",wTime);    // put in two spaces.
    } else if (wTime<100) {               // Else if its less than 100..               
      snprintf (wText,5," %d",wTime);     // One space.
    } else {                              // Else.. (>99) 
      snprintf (wText,5,"%d",wTime);      // No spaces.
    }
  }
  strcpy(tempStrBuff,"Water time : ");
  strcat(tempStrBuff,wText);
  strcat(tempStrBuff," s");
}

 
void controlPanel::soakTimeText(long sTime) {

    char sText[20];

  if (sTime<0) {                         
    strcpy(sText,"---");
  } else {
    sTime = round(sTime/1000.0);          // Comes in as ms, we want seconds.
    if (sTime<10) {                       // If its 1-9..
      snprintf (sText,5,"  %d",sTime);    // put in two spaces.
    } else if (sTime<100) {               // Else if its less than 100..               
      snprintf (sText,5," %d",sTime);     // One space.
    } else {                              // Else.. (>99) 
      snprintf (sText,5,"%d",sTime);      // No spaces.
    }
  }
  strcpy(tempStrBuff,"Soak time  : ");
  strcat(tempStrBuff,sText);
  strcat(tempStrBuff," s");
}


void controlPanel::checkMoisture(void) {
  
  byte  byteBuff;

  mMoistureLight->setColor(LC_WHITE); 
  byteBuff = readMoisture;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==1) {
    ourComPort.readBuff(&byteBuff);
    moistureText(byteBuff);
    mMoistureLabel->setValue(tempStrBuff);
    mMoistureLight->setColor(LC_GREEN);
    mMoistureLabel->setNeedRefresh();
  } else {
    ourComPort.dumpBuff();
    mMoistureLight->setColor(LC_RED);
  }
}


void controlPanel::checkState(void) {

  byte  byteBuff;

  mStateLight->setColor(LC_WHITE); 
  byteBuff = readState;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==1) {
    ourComPort.readBuff(&byteBuff);
    stateText(byteBuff);
    mStateLight->setColor(LC_GREEN);
  } else {
    ourComPort.dumpBuff();
    stateText(-1);
    mStateLight->setColor(LC_RED);
  }
  mStateLabel->setValue(tempStrBuff);  
}


void controlPanel::checkDryLimit(void) {

  byte  byteBuff;

  mDryLimitLight->setColor(LC_WHITE); 
  byteBuff = readDryLimit;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==1) {
    ourComPort.readBuff(&byteBuff);
    mLastLimit = byteBuff;
    dryLimitText(byteBuff);
    mDryLimitLabel->setValue(tempStrBuff);
    mDryLimitLight->setColor(LC_GREEN);
    mDryLimitLabel->setNeedRefresh();
  } else {
    ourComPort.dumpBuff();
    mDryLimitLight->setColor(LC_RED);
  }
}


void controlPanel::checkWaterTime(void) {

  byte  byteBuff;
  long  longBuff;

  mWaterTimeLight->setColor(LC_WHITE); 
  byteBuff = readWaterTime;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  byteBuff = ourComPort.haveBuff();
  Serial.println(byteBuff);
  Serial.println(sizeof(long));
  if (byteBuff==sizeof(long)) {
    ourComPort.readBuff((byte*)&longBuff);
    mLastWTime = longBuff/1000;
    waterTimeText(longBuff);
    mWaterTimeLabel->setValue(tempStrBuff);
    mWaterTimeLight->setColor(LC_GREEN);
  } else {
    ourComPort.dumpBuff();
    waterTimeText(-1);
    mWaterTimeLabel->setValue(tempStrBuff);
    mWaterTimeLight->setColor(LC_RED);
  }
}


void controlPanel::checkSoakTime(void) {

  byte  byteBuff;
  long  longBuff;

  mSoakTimeLight->setColor(LC_WHITE); 
  byteBuff = readSoakTime;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff==sizeof(long)) {
    ourComPort.readBuff((byte*)&longBuff);
    mLastSTime = longBuff/1000;
    soakTimeText(longBuff);
    mSoakTimeLabel->setValue(tempStrBuff);
    mSoakTimeLight->setColor(LC_GREEN);
  } else {
    ourComPort.dumpBuff();
    soakTimeText(-1);
    mSoakTimeLabel->setValue(tempStrBuff);
    mSoakTimeLight->setColor(LC_RED);
  }
}


void controlPanel::checkName(void) { 

  byte  byteBuff;
  char  nameBuff[NAME_BUFF_BYTES];

  mNameLight->setColor(LC_WHITE); 
  byteBuff = readName;
  ourComPort.readErr();
  ourComPort.sendBuff(&byteBuff,1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  byteBuff = ourComPort.haveBuff();
  if (byteBuff&&byteBuff<=NAME_BUFF_BYTES) {
    ourComPort.readBuff((byte*)&nameBuff);
    mNameLabel->setValue(nameBuff);
    mSoakTimeLabel->setNeedRefresh();
    mNameLight->setColor(LC_GREEN);
  } else {
    ourComPort.dumpBuff();
    mNameLight->setColor(LC_RED);
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
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
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
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  if (ourComPort.haveBuff()) {
    ourComPort.dumpBuff();
  }
}


void controlPanel::doComSetWTime(float wTime) { 

  byte    byteBuff[sizeof(long)+1];
   long   longTime;
   long*  longPtr;
  
  if (wTime<0) wTime = 0;
  else if (wTime>MAX_WTIME) wTime = MAX_WTIME;
  longTime = round(wTime*1000);
  byteBuff[0] = (byte)setWaterTime;
  longPtr = (long*)(&(byteBuff[1]));
  *longPtr = longTime;
  ourComPort.sendBuff(byteBuff,sizeof(long)+1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  if (ourComPort.haveBuff()) {
    ourComPort.dumpBuff();
  }
}



void controlPanel::doComSetSTime(float sTime) {

  byte    byteBuff[sizeof(long)+1];
  long    longTime;
  long*   longPtr;
  
  if (sTime<0) sTime = 0;
  else if (sTime>MAX_STIME) sTime = MAX_STIME;
  longTime = round(sTime*1000);
  byteBuff[0] = (byte)setSoakTime;
  longPtr = (long*)(&(byteBuff[1]));
  *longPtr = longTime;
  ourComPort.sendBuff(byteBuff,sizeof(long)+1,true);
  while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(SLEEP_TIME);}
  if (ourComPort.haveBuff()) {
    ourComPort.dumpBuff();
  }
}

          
void controlPanel::loop(void) {

  if (mReadTimer.ding()&&!selectedVal) {                        
    switch (dataChoice) {
      case dataStart      : break;
      case dataName       : checkName();  break;
      case dataState      : checkState();  break;
      case dataMoisture   : checkMoisture();  break;
      case dataDryLimit   : checkDryLimit();  break;
      case dataWaterTime  : checkWaterTime();  break;
      case dataSoakTime   : checkSoakTime();  break;
      case dataEnd        : dataChoice = dataStart; break;
    }
    dataChoice = (dataSelect)(dataChoice + 1);
    mReadTimer.start();
  }
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
