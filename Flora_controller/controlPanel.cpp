#include "controlPanel.h"
//#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"


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


controlPanel::controlPanel(void) : panel(controlApp) {

  mLastLimit = 50;
  mLastWTime = 15;
  mLastSTime = 30;
}


controlPanel::~controlPanel(void) { }


void controlPanel::setup(void) {

    int       traceY;
    colorObj  labelColor;

    labelColor.setColor(LC_WHITE);
    mReadTimer.setTime(READ_TIME);

    traceY = 30;
    
    mNameLabel = new fontLabel(10,traceY,220,18);
    mNameLabel->setColors(&white,&black);
    mNameLabel->setJustify(TEXT_CENTER);
    addObj(mNameLabel);
    
    mNameLight = new colorRect(0,traceY+4,6,6);
    mNameLight->setColor(LC_RED);
    addObj(mNameLight);

    /*
    traceY = traceY + LINE_SPACE;
    stateText(-1);
    mStateLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mStateLabel->setColors(&labelColor,&black);
    addObj(mStateLabel);

    mStateLight = new colorRect(0,traceY+4,6,6);
    mStateLight->setColor(LC_RED);
    addObj(mStateLight);

    tempText(-1);
    traceY = traceY + LINE_SPACE;
    mTempLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mTempLabel->setColors(&labelColor,&black);
    addObj(mTempLabel);

    mTempLight = new colorRect(0,traceY+4,6,6);
    mTempLight->setColor(LC_RED);
    addObj(mTempLight);
    */
    
    moistureText(-1);
    traceY = traceY + LINE_SPACE;
    mMoistureLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mMoistureLabel->setColors(&labelColor,&black);
    addObj(mMoistureLabel);

    mMoistureLight = new colorRect(0,traceY+4,6,6);
    mMoistureLight->setColor(LC_RED);
    addObj(mMoistureLight);
    
    dryLimitText(-1);
    traceY = traceY + (2*LINE_SPACE);
    mDryLimitLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mDryLimitLabel->setColors(&labelColor,&black);
    addObj(mDryLimitLabel);

    mDryLimitLight = new colorRect(0,traceY+4,6,6);
    mDryLimitLight->setColor(LC_RED);
    addObj(mDryLimitLight);
    
    mDryLimitBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,DRY_LIMIT,this);
    addObj(mDryLimitBtn);
      
    waterTimeText(-1);
    traceY = traceY + LINE_SPACE;
    mWaterTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mWaterTimeLabel->setColors(&labelColor,&black);
    addObj(mWaterTimeLabel);

    mWaterTimeLight = new colorRect(0,traceY+4,6,6);
    mWaterTimeLight->setColor(LC_RED);
    addObj(mWaterTimeLight);
    
    mWaterTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,W_TIME,this);
    addObj(mWaterTimeBtn);
    
    soakTimeText(-1);
    traceY = traceY + LINE_SPACE;
    mSoakTimeLabel = new label(10,traceY,220,14,tempStrBuff,2);
    mSoakTimeLabel->setColors(&labelColor,&black);
    addObj(mSoakTimeLabel);

    mSoakTimeLight = new colorRect(0,traceY+4,6,6);
    mSoakTimeLight->setColor(LC_RED);
    addObj(mSoakTimeLight);
    
    mSoakTimeBtn  = new selectBtn(SELECT_X,traceY-SELECT_YOFFS,SELECT_W,SELECT_H,S_TIME,this);
    addObj(mSoakTimeBtn);

    mloggingLight = new bmpFlasher(100,traceY+23,10,10,ON_GREEN_BMP,OFF_GREEN_BMP);
    mloggingLight->setTimes(1000, 1500);
    mloggingLight->setBlink(false);
    addObj(mloggingLight);
    
   
    mlogfileSizeLabel = new label(115,traceY+23,100,8,"",1);
    mlogfileSizeLabel->setColors(&labelColor,&black);
    addObj(mlogfileSizeLabel);
    
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
    mOkBtn = new okBtn(125,traceY,32,32,this);
    mOkBtn->select(false);
    addObj(mOkBtn);

    mCancelBtn = new cancelBtn(188,traceY,32,32,this);
    mCancelBtn->select(false);
    addObj(mCancelBtn);
}


void controlPanel::tempText(int temp) {

  char tText[5];
  
  if (temp<0) {
    strcpy(tText,"---");
  } else if (temp<10) {
    snprintf (tText,5,"  %d",temp);
  } else if (temp<100) {
    snprintf (tText,5," %d",temp);
  } else {
    snprintf (tText,5,"%d",temp);
  }
  strcpy(tempStrBuff,"Temp       : ");
  strcat(tempStrBuff,tText);
  strcat(tempStrBuff," C");
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
  
  if (moisture<0) {
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

          
void controlPanel::loop(void) { }

  
void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}


void controlPanel::closing(void) { }
