#include "controlPanel.h"


char tempStrBuff[256];

// *****************************************************
// ******************    pumpBtn   *********************
// *****************************************************

pumpBtn::pumpBtn(int x, int y,int width, int height)
  :baseButton("Pump",x,y,width,height) {  }

  
pumpBtn::~pumpBtn(void) {  }

void  pumpBtn::doAction(event* inEvent,point* locaPt) {

  byte outVal;
  
  Serial.println("doAction()");
  switch(inEvent->mType) {
    case touchEvent : 
      Serial.println("PUMP ON!!");
      outVal = pumpOn;
      ourComPort.sendBuff(&outVal,1,false);
    break;
    case liftEvent :
      Serial.println("PUMP OFF!!");
      outVal = pumpOff;
      ourComPort.sendBuff(&outVal,1,false);
    break;
  }
}


// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************

controlPanel::controlPanel(void) : panel(controlApp) { }


controlPanel::~controlPanel(void) { }


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


void controlPanel::waterTimeText(unsigned long wTime) {

  char wText[20];

  wTime = round(wTime/1000.0);              // Comes in as ms, we want seconds.
  if (wTime<0) {
    wTime=0;
  }
  if (!wTime) {                         
    strcpy(wText,"---");
  } else if (wTime<10) {                    // If its 1-9..
    snprintf (wText,5,"  %d",wTime);    // put in two spaces.
  } else if (wTime<100) {                   // Else if its less than 100..               
    snprintf (wText,5," %d",wTime);     // One space.
  } else {                                  // Else.. (>99) 
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
  } else if (sTime<10) {                    // If its 1-9..
    snprintf (sText,5,"  %d",sTime);    // put in two spaces.
  } else if (sTime<100) {                   // Else if its less than 100..               
    snprintf (sText,5," %d",sTime);     // One space.
  } else {                                  // Else.. (>99) 
    snprintf (sText,5,"%d",sTime);      // No spaces.
  }
  strcpy(tempStrBuff,"Soak time  : ");
  strcat(tempStrBuff,sText);
  strcat(tempStrBuff," sec.");
}


void controlPanel::setup(void) {

    mReadTimer.setTime(500);
    pumpBtn* btn = new pumpBtn(20,90,80,20);
    addObj(btn);
    
    moistureText(0);
    mMoistureLabel = new label(10,30,220,14,tempStrBuff,2);
    mMoistureLabel->setColors(&blue,&black);
    addObj(mMoistureLabel);

    waterTimeText(0);
    mWaterTimeLabel = new label(10,50,220,14,tempStrBuff,2);
    mWaterTimeLabel->setColors(&blue,&black);
    addObj(mWaterTimeLabel);

    soakTimeText(0);
    mSoakTimeLabel = new label(10,70,220,14,tempStrBuff,2);
    mSoakTimeLabel->setColors(&blue,&black);
    addObj(mSoakTimeLabel);
}


void controlPanel::loop(void) {

  byte          byteBuff;
  unsigned long longBuff;
  
  if (mReadTimer.ding()) {
    byteBuff = readMoisture;
    ourComPort.readErr();
    ourComPort.setReplyTimeout(500);
    ourComPort.sendBuff(&byteBuff,1,true);
    while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
    byteBuff = ourComPort.haveBuff();
    if (byteBuff==1) {
      ourComPort.readBuff(&byteBuff);
      moistureText(byteBuff);
      mMoistureLabel->setValue(tempStrBuff);
      mMoistureLabel->setNeedRefresh();
    }

    
    byteBuff = readWaterTime;
    ourComPort.readErr();
    ourComPort.sendBuff(&byteBuff,1,true);
    while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
    byteBuff = ourComPort.haveBuff();
    if (byteBuff==sizeof(unsigned long)+1) {
      ourComPort.readBuff((byte*)&longBuff);
      waterTimeText(longBuff);
      mWaterTimeLabel->setValue(tempStrBuff);
      mWaterTimeLabel->setNeedRefresh();
    } else {
      Serial.print("wrong got back ");
      Serial.print(byteBuff);
      Serial.print(" bytes. ");
    }
    
    byteBuff = readSoakTime;
    ourComPort.readErr();
    ourComPort.setReplyTimeout(500);
    ourComPort.sendBuff(&byteBuff,1,true);
    while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
    byteBuff = ourComPort.haveBuff();
    if (byteBuff==sizeof(unsigned long)+1) {
      ourComPort.readBuff((byte*)&longBuff);
      soakTimeText(longBuff);
      mSoakTimeLabel->setValue(tempStrBuff);
      mSoakTimeLabel->setNeedRefresh();
    }
    
    mReadTimer.start();
  }
}


void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}

  
void controlPanel::close(void) { }


void controlPanel::closing(void) { }
