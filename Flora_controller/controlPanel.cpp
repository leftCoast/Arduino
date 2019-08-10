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
    strcpy(moistText,"--");
  } else if (moisture<10) {
    snprintf (moistText,5," %d",moisture);
  } else {
    snprintf (moistText,5,"%d",moisture);
  }
  strcpy(tempStrBuff,"Moisture : ");
  strcat(tempStrBuff,moistText);
  strcat(tempStrBuff,"%");
}


void controlPanel::setup(void) {

    mReadTimer.setTime(500);
    pumpBtn* btn = new pumpBtn(20,60,80,40);
    addObj(btn);
    
    moistureText(0);
    mMoistureLabel = new label(10,30,200,14,tempStrBuff,2);
    mMoistureLabel->setColors(&blue,&black);
    addObj(mMoistureLabel);
}


void controlPanel::loop(void) {

  byte buff;

  if (mReadTimer.ding()) {
    buff = readMoisture;
    ourComPort.readErr();
    ourComPort.setReplyTimeout(500);
    ourComPort.sendBuff(&buff,1,true);
    while(!ourComPort.haveBuff()&&!ourComPort.readErr()) { sleep(10);}
    buff = ourComPort.haveBuff();
    if (buff==1) {
      ourComPort.readBuff(&buff);
      Serial.println(buff);
      moistureText(buff);
      mMoistureLabel->setValue(tempStrBuff);
      mMoistureLabel->setNeedRefresh();
    }
    mReadTimer.start();
  }
}


void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}

  
void controlPanel::close(void) { }


void controlPanel::closing(void) { }
