#include "controlPanel.h"
#include "Fonts/FreeSans9pt7b.h"
#include <bmpObj.h> 
#include <debug.h>

#define DRY_LIMIT     1
#define W_TIME        2
#define S_TIME        3

#define SELECT_X      162
#define SELECT_W      45
#define SELECT_H      20
#define SELECT_YOFFS  3




char tempStrBuff[256];


// *****************************************************
// ******************   selectBtn   ********************
// *****************************************************


selectBtn::selectBtn(int x,int y,int width,int height,int inChoice,controlPanel* inPanel)
  :baseButton(NULL,x,y,width,height) {
    
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

  if (ourComPort.getOnline()) {
    if (inEvent->mType==touchEvent) {
      if (selectedVal) {
        selectedVal->setNeedRefresh();
      }
      selectedVal = this;
      switch (mChoice) {
        case DRY_LIMIT  :
          mPanel->mSlider->setLabel(mPanel->mSliderPLabel);
          mPanel->mSlider->setup(DEF_SLIDER_KNOB_W,DEF_SLIDER_LINE_H,0,100);
          mPanel->mSlider->setValue(ourComPort.getLimit());
        break;
        case W_TIME     :
          mPanel->mSlider->setLabel(mPanel->mSliderTLabel);
          mPanel->mSlider->setup(DEF_SLIDER_KNOB_W,DEF_SLIDER_LINE_H,0,MAX_WTIME);
          mPanel->mSlider->setValue(ourComPort.getWaterTime());
        break;
        case S_TIME     :
          mPanel->mSlider->setLabel(mPanel->mSliderTLabel);
          mPanel->mSlider->setup(DEF_SLIDER_KNOB_W,DEF_SLIDER_LINE_H,0,MAX_STIME);
          mPanel->mSlider->setValue(ourComPort.getSoakTime());
        break;
      }
      mPanel->mSlider->select(true);
      mPanel->mOkBtn->select(true);
      mPanel->mCancelBtn->select(true);
    }
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


// We're being turned on or off. What do we do?!
void editSlider::select(bool onOff) {
  
  int value;
  
  slider::select(onOff);                      // Let the ancesters do their thing first.        
  ourComPort.runUpdates(!onOff);              // If we're going on, we want updates off. And vice versa.
  if (onOff) {                                // If we're turning on..
    value = round(mPos2ValMap.Map(mNewLoc));  // We have the initial value loaded. And you'd think we'd just call setText(). 
    mLabel->setValue(value);                  // But no.. Won't work because its not rolled in position yet. So, just for now, we do this.
  } else {                                    // Else we're turing off..
    if (mLabel) mLabel->setValue("");         // Clear our label of the screen.
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
  : baseIconButton(x,y,width,height,"/icons/check32.bmp") {
    
  mPanel = inPanel;
}

   
okBtn::~okBtn(void) {  }


void okBtn::doAction(event* inEvent,point* locaPt) {

  if (inEvent->mType==touchEvent) {
    screen->fillRect((rect*)this,&white); // Give it a flash.
  } else if (inEvent->mType==liftEvent) {
    mPanel->mSlider->select(false);
    mPanel->mCancelBtn->select(false);
    select(false);
    if (selectedVal) {
      switch (selectedVal->getChoice()) {
        case DRY_LIMIT  : ourComPort.setLimitReg(round(mPanel->mSlider->getValue())); break;
        case W_TIME     : ourComPort.setWTimeReg(round(mPanel->mSlider->getValue())); break;
        case S_TIME     : ourComPort.setSTimeReg(round(mPanel->mSlider->getValue())); break;
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
  : baseIconButton(x,y,width,height,"/icons/x32.bmp") {

  mPanel = inPanel;
}

   
cancelBtn::~cancelBtn(void) {  }


void cancelBtn::doAction(event* inEvent,point* locaPt) {

  if (inEvent->mType==touchEvent) {
    screen->fillRect((rect*)this,&white); // Give it a flash.
  } else if (inEvent->mType==liftEvent) {
    mPanel->mSlider->select(false);
    mPanel->mOkBtn->select(false);
    select(false);
    if (selectedVal) selectedVal->setNeedRefresh();
    selectedVal = NULL;
  }
}


     
// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************


#define LINE_SPACE  20
#define TOP_Y       30
#define LABEL_W     138
#define LABEL_H     18
#define LINE_SPACE  20

#define LIVE_X        LABEL_W+5
#define LIVE_W        90


controlPanel::controlPanel(void) : panel(controlApp) {  }


controlPanel::~controlPanel(void) { }


void controlPanel::setup(void) {

  int traceY;
  int stepY;
  
  traceY = TOP_Y;
  stepY  = LINE_SPACE;

  nameText* mNameLabel = new nameText(10,traceY,230,18);
  addObj(mNameLabel);


  
  traceY = traceY + 2 * stepY;
  bmpObj* limit = new bmpObj(0,traceY,LABEL_W,LABEL_H,LIMIT_BMP);
  limit->begin();
  addObj(limit);

  limitText*  lText = new limitText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(lText);

  mDryLimitBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,DRY_LIMIT,this);
  addObj(mDryLimitBtn);
  
  traceY = traceY + stepY;
  bmpObj* waterT = new bmpObj(0,traceY,LABEL_W,LABEL_H,WATER_TIME_BMP);
  waterT->begin();
  addObj(waterT);

  wTimeText*  wtText = new wTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(wtText);

  mWaterTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,W_TIME,this);
  addObj(mWaterTimeBtn);

  traceY = traceY + stepY;
  bmpObj* soakT = new bmpObj(0,traceY,LABEL_W,LABEL_H,SOAK_TIME_BMP);
  soakT->begin();
  addObj(soakT);

  sTimeText*  sText = new sTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(sText);

  mSoakTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,S_TIME,this);
  addObj(mSoakTimeBtn);

  traceY = traceY + 2*LINE_SPACE;
  mSliderTLabel  = new timeText(120,traceY,100,14);
  mSliderTLabel->setColors(&white,&black);
  addObj(mSliderTLabel);

  mSliderPLabel  = new percentText(120,traceY,100,14);
  mSliderPLabel->setColors(&white,&black);
  addObj(mSliderPLabel);
  
  traceY = traceY + (LINE_SPACE);
  mSlider = new editSlider(20,traceY,200,20);
  mSlider->select(false);
  addObj(mSlider);

  traceY = traceY + 2*LINE_SPACE;
  mOkBtn = new okBtn(125,traceY,32,32,this);
  mOkBtn->select(false);
  addObj(mOkBtn);

  mCancelBtn = new cancelBtn(188,traceY,32,32,this);
  mCancelBtn->select(false);
  addObj(mCancelBtn);
}


void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}
