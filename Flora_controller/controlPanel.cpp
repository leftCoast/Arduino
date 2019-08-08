#include "controlPanel.h"



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


void controlPanel::setup(void) {

    pumpBtn* btn = new pumpBtn(40,40,80,40);
    addObj(btn);
}


void controlPanel::loop(void) {  }


void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}

  
void controlPanel::close(void) { }


void controlPanel::closing(void) { }
