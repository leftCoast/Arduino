#include "controlPanel.h"


#define NO_SELECTION  0
#define DRY_LIMIT     1
#define W_TIME        2
#define S_TIME        3

#define SELECT_X      162
#define SELECT_W      45
#define SELECT_H      20
#define SELECT_YOFFS  3

#define READ_TIME 500

enum commandChoices { comIdle  ,comPumpOn, comPumpOff };
commandChoices  commandChoice = comIdle;

char tempStrBuff[256];



// *****************************************************
// ******************    pumpBtn   *********************
// *****************************************************


pumpBtn::pumpBtn(int x, int y,int width, int height)
  :baseButton("Water",x,y,width,height) { 
    
  mOnOff = true;
}

  
pumpBtn::~pumpBtn(void) {  }


void  pumpBtn::doAction(event* inEvent,point* locaPt) {

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


selectBtn::selectBtn(int x,int y,int width,int height,int inChoice)
  :baseButton("",x,y,width,height) {  mChoice = inChoice; }

  
selectBtn::~selectBtn(void) {  }


int selectBtn::getChoice(void) { return mChoice; }


void selectBtn::drawSelf(void) {

  if (selectedVal==this) {
    screen->drawRoundRect(x, y, width, height, 4, &red);
  } else {
    screen->drawRoundRect(x, y, width, height, 4, &black);
  }
}


void  selectBtn::doAction(event* inEvent,point* locaPt) {
  
  if (inEvent->mType==touchEvent) {
    if (selectedVal) {
      selectedVal->setNeedRefresh();
    }
    selectedVal = this;
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


okBtn::okBtn(int x, int y,int width, int height)
  : baseButton("Ok",x,y,width,height) { activeBColor.setColor(&green); }

   
okBtn::~okBtn(void) {  }


void okBtn::doAction(event* inEvent,point* locaPt) {

  
}



// *****************************************************
// ****************    cancelBtn    ********************
// *****************************************************


cancelBtn::cancelBtn(int x, int y,int width, int height)
  : baseButton("Cancel",x,y,width,height) { activeBColor.setColor(&red); }

   
cancelBtn::~cancelBtn(void) {  }


void cancelBtn::doAction(event* inEvent,point* locaPt) {

  
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

    mDryLimitBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,DRY_LIMIT);
    addObj(mDryLimitBtn);
      
    waterTimeText(0);
    traceY = traceY + LINE_SPACE;
    mWaterTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mWaterTimeLabel->setColors(&labelColor,&black);
    addObj(mWaterTimeLabel);

    mWaterTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,W_TIME);
    addObj(mWaterTimeBtn);
    
    soakTimeText(0);
    traceY = traceY + LINE_SPACE;
    mSoakTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mSoakTimeLabel->setColors(&labelColor,&black);
    addObj(mSoakTimeLabel);

    mSoakTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,S_TIME);
    addObj(mSoakTimeBtn);

    traceY = traceY + 2*LINE_SPACE;
    mSliderLabel  = new label(120,traceY,100,14,"",1);
    mSliderLabel->setColors(&labelColor,&black);
    addObj(mSliderLabel);
    
    traceY = traceY + (LINE_SPACE);
    mSlider = new editSlider(20,traceY,200,20);
    mSlider->setLabel(mSliderLabel);
    mSlider->setValue(50);
    addObj(mSlider);

    traceY = traceY + 2*LINE_SPACE;
    mOkBtn = new okBtn(90,traceY,35,20);
    addObj(mOkBtn);

    mCancelBtn = new cancelBtn(150,traceY,80,20);
    addObj(mCancelBtn);
     
    traceY = traceY + 3*LINE_SPACE;
    pumpBtn* btn = new pumpBtn(20,traceY,80,20);
    addObj(btn);
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
    strcpy(dryLimitText," 0");
  } else if (dryLimit<10) {
    snprintf (dryLimitText,5," %d",dryLimit);
  } else {
    snprintf (dryLimitText,5,"%d",dryLimit);
  }
  strcpy(tempStrBuff,"Dry limit  :  ");
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
  
  /* 
  switch (commandChoice) {
      case comIdle    : break;
      case comPumpOn  : doComPump(true);  break;
      case comPumpOff : doComPump(false); break;
  }
  */
}


void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}

  
void controlPanel::close(void) { }


void controlPanel::closing(void) { }
