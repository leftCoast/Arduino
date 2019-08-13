#include "controlPanel.h"

enum commandChoices { comIdle  ,comPumpOn, comPumpOff };
commandChoices  commandChoice = comIdle;

char tempStrBuff[256];

// *****************************************************
// ******************    pumpBtn   *********************
// *****************************************************

pumpBtn::pumpBtn(int x, int y,int width, int height)
  :baseButton("Water",x,y,width,height) {  }

  
pumpBtn::~pumpBtn(void) {  }

void  pumpBtn::doAction(event* inEvent,point* locaPt) {

  byte outVal;

  if (commandChoice==comIdle) {
    switch(inEvent->mType) {
      case touchEvent : commandChoice = comPumpOn; break;
      case liftEvent : commandChoice = comPumpOff; break;
    }
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
    
    mReadTimer.setTime(500);
    ourComPort.setReplyTimeout(100);

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

    waterTimeText(0);
    traceY = traceY + LINE_SPACE;
    mWaterTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mWaterTimeLabel->setColors(&labelColor,&black);
    addObj(mWaterTimeLabel);

    soakTimeText(0);
    traceY = traceY + LINE_SPACE;
    mSoakTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mSoakTimeLabel->setColors(&labelColor,&black);
    addObj(mSoakTimeLabel);

    traceY = traceY + (2*LINE_SPACE);
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
  strcat(tempStrBuff," sec.");
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

  if (mReadTimer.ding()) {                           
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
  switch (commandChoice) {
      case comIdle    : break;
      case comPumpOn  : doComPump(true);  break;
      case comPumpOff : doComPump(false); break;
  }
}




void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}

  
void controlPanel::close(void) { }


void controlPanel::closing(void) { }
