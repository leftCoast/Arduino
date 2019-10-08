#include "UI.h"
#include <adafruit_684_Obj.h>
#include <bmpPipe.h>
#include <colorObj.h>
#include <blinker.h>
#include "parameters.h"
#include <liveText.h>

#define OLED_CS     10
#define OLED_RST    6
#define OLED_SDCS   4

UI      ourDisplay;
blinker idleLight(13,80,2000);
 
// Set percent vlaues from 0..100 as a 3 char int. -1 gives "---"

percView::percView(int x,int y)
  : label(x,y,36,8,"---",2) {
  setColors(&white,&black); 
}


percView::~percView(void) { }


void percView::setPercent(float percent) {

  int   intPercent;
  char  pText[6];
  
  if (percent<0) {                         
    strcpy(pText,"---");
  } else {
    intPercent = round(percent);              // Comes in as a percent float.
    if (intPercent<10) {                      // If its 1-9..
      snprintf (pText,5,"  %d",intPercent);   // put in two spaces.
    } else if (intPercent<100) {              // Else if its less than 100..               
      snprintf (pText,5," %d",intPercent);    // One space.
    } else {                                  // Else.. (>99) 
      snprintf (pText,5,"%d",intPercent);     // No spaces.
    }
  }
  setValue(pText);
}



// *****************************************************


stateView::stateView(int x,int y) 
  : label(x,y,96-x,8,"Startup",2) {
  setColors(&white,&black);
}


stateView::~stateView(void) {  }


void  stateView::setState(weDo inState) {

  switch(inState) {
    case sitting    : setValue("Reading"); break;
    case watering   : setValue("Watering"); break;
    case soaking    : setValue("Soaking"); break;
  }
}



// *****************************************************


logInd::logInd(int x,int y)
  :liveText(x,y,96,8) {

  setTextSize(1);
  setValue("Logging");
  transp = true;
  setJustify(TEXT_CENTER);
  hold();
  addAColor(0,&black);
  addAColor(250,&red);
  addAColor(1000,&red);
  addAColor(1050,&black);
  addAColor(1500,&black);
  loop = true;
}

                
logInd::~logInd(void) {  }


void logInd::logging(bool onOff) {
  
  if (onOff) {
    release();
    loop = true;
  } else {
    loop = false;
  }
}

          
// *****************************************************


// Our output. If we have a screen, we use it. If not? We can flash the light.


UI::UI(void) 
  : timeObj(500) {
    
  mHaveScreen = false;
  mColorMap = NULL;
}


UI::~UI(void) { if (mColorMap) delete(mColorMap); }


// We can't tell weather we have a screen or not. So we'll look for the SD drive.
void UI::begin(void) {

  hookup();
  mHaveScreen = SD.begin(OLED_SDCS);
  dataLog::begin(mHaveScreen);  // Hardware's been checked. Fire the logger up.
  if (mHaveScreen) {
    mHaveScreen = mHaveScreen && initScreen(ADAFRUIT_684,OLED_CS,OLED_RST,INV_LANDSCAPE);
    if (mHaveScreen) {
       
       mLastMoist = -1;
       mLastLimit = -1;
       mLastState = weAre;
      
      screen->fillScreen(&black);

      mColorMap = new colorMultiMap();
      mWetColor.setColor(LC_BLUE);
      mWetColor.blend(&green,40);
      mDryColor.setColor(LC_YELLOW);
      mDryColor.blend(&red,40);
      mDryColor.blend(&white,40);
      
      int yPos = 0;

      mMoisture = new percView(5,yPos);
      mMoisture->setPercent(mLastMoist);
      viewList.addObj(mMoisture);
      
      mLimit = new percView(53,yPos);
      mLimit->setPercent(mLastLimit);
      viewList.addObj(mLimit);
  
      mSlash = new label(41,yPos,96,16,"/",2);
      mSlash->setColors(&white);
      viewList.addObj(mSlash);
  
      yPos = yPos + 20;
      mKey = new label(9,yPos,96,16,"Reading/Limit",1);
      mKey->setColors(&white);
      viewList.addObj(mKey);

      mLoggingInd = new logInd(0,yPos+10);
      viewList.addObj(mLoggingInd);
      mLoggingInd->logging(false);

      yPos = yPos + 20;                      //"100/100"
      mState = new stateView(0,yPos);
      mState->setState(mLastState);
      viewList.addObj(mState);

      start();
    } 
  }
  if (!mHaveScreen) {
    pinMode(13, OUTPUT);
    idleLight.setBlink(true);                             // Start up our running light. Its all we got.
  }
}


void UI::setColorMap(int limit) {
  
  mColorMap->clearMap();
  if (limit>=100) {
    mColorMap->addColor(100,&white);
    mColorMap->addColor(0,&mDryColor); 
  } else if (limit==0) {
    mColorMap->addColor(100,&mWetColor);
    mColorMap->addColor(0,&white);
  } else if (limit>10) {
    mColorMap->addColor(100,&mWetColor);
    mColorMap->addColor(limit-10,&mDryColor);
    mColorMap->addColor(limit,&white);
  } else {
    mColorMap->addColor(100,&mWetColor);
    mColorMap->addColor(0,&mDryColor);
    mColorMap->addColor(limit,&white);
  }
}


void UI::sensorDeath(void) {

  crash = true;
  if (mHaveScreen) {
    bmpPipe bomb(rect(0,0,96,64));
    bomb.openPipe("/bomb9664.bmp");
    bomb.drawBitmap(0,0);
  }
  pinMode(13, OUTPUT);
  while(crash) {
    digitalWrite(13,HIGH);
    delay(20);
    digitalWrite(13,LOW);
    delay(100);
  }              
}


void UI::idle(void) {

  bool  refresh;

  
  if (mHaveScreen && ding()) {
    refresh = false;
    if (ourParamObj.getDryLimit()!=mLastLimit) {
      mLimit->setPercent(ourParamObj.getDryLimit());
      mLastLimit = ourParamObj.getDryLimit();
      refresh = true;
    }
    if (moisture!=mLastMoist||refresh) {
      setColorMap(ourParamObj.getDryLimit());
      mMoisture->setColors(&(mColorMap->Map(moisture)),&black);
      mMoisture->setPercent(moisture);
      mLastMoist = moisture;
    }
    if (weAre!=mLastState) {
      mState->setState(weAre);
      mLastState = weAre;
    }
    if (isLogging()==mLoggingInd->holding) {  // If they match,
      mLoggingInd->logging(isLogging());      // fix it.
    }                                         // Otherwise, leave it alone!
    start();
  }
};
