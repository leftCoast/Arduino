#include "UI.h"
#include <adafruit_684_Obj.h>
#include <bmpPipe.h>
#include <colorObj.h>
#include <blinker.h>
#include "parameters.h"
#include <liveText.h>
#include <offscreen.h>
#include "sleepTools.h"



#define OLED_CS      10
#define OLED_RST     6
#define OLED_SDCS    4
#define TAP_PIN      15
#define TAP_SUMTIME  20
#define TAP_NUMDATA  3
#define TAP_VAL      10
#define AWAKE_MS     8000
#define DIMMING_MS   3000


UI        ourDisplay;
blinker   idleLight(13,80,2000);
offscreen ourBlitPort;

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
  : label(x,y,96-x,18,"Startup",2) {

  mPerc = 0;
  setColors(&white);
  setJustify(TEXT_CENTER);
  
  mBMap.setSize(width,height);
 
  mReadColor.setColor(&black);
  
  mWaterColor.setColor(&blue);
  mWaterColor.blend(&black,20);
  mWaterColor.blend(&green,40);
  
  mSoakColor.setColor(&yellow);
  mSoakColor.blend(&black,40);
  mSoakColor.blend(&red,20);
}


stateView::~stateView(void) {  }


void  stateView::setState(weDo inState) {

  switch(inState) {
    case sitting    : setValue("Reading"); break;
    case watering   : setValue("Watering"); break;
    case soaking    : setValue("Soaking"); break;
  }
  mState = inState;
}


void stateView::drawSelf(void) {

  float timeFraction;
  rect  colorRect(this);

  ourBlitPort.beginDraw(&mBMap,screen->gX(x),screen->gY(y));
  switch(mState) {
    case sitting    : 
      screen->fillRect(this,&mReadColor);
      x = x+width/16;
      y = y+1;
      label::drawSelf();
      x = x-96/16;
      y = y-1;
    break;
    case watering   : 
      screen->fillRect(&colorRect,&black);
      timeFraction = waterTime->getFraction();
      colorRect.width = colorRect.width - (colorRect.width * timeFraction);
      screen->fillRect(&colorRect,&mWaterColor);
      y = y+1;
      label::drawSelf();
      y = y-1;
    break;
    case soaking    :
      screen->fillRect(&colorRect,&black);
      timeFraction = soakTime->getFraction();
      colorRect.width = colorRect.width - (colorRect.width * timeFraction);
      screen->fillRect(&colorRect,&mSoakColor);
      x = x+width/16;
      y = y+1;
      label::drawSelf();
      x = x-96/16;
      y = y-1;
    break;
  }
  ourBlitPort.endDraw();
  screen->blit(x,y,&mBMap);
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
  : timeObj(250) {
    
  mHaveScreen = false;
  mColorMap = NULL;
  mTSensor = NULL;
}


UI::~UI(void) {
   if (mColorMap) delete(mColorMap);
   if (mTSensor) delete(mTSensor);
}


// We can't tell weather we have a screen or not. So we'll look for the SD drive.
void UI::begin(void) {

   hookup();
   mHaveScreen = SD.begin(OLED_SDCS);
   dataLog::begin(mHaveScreen);  // Hardware's been checked. Fire the logger up.
   if (mHaveScreen) {
      mHaveScreen = initScreen(ADAFRUIT_684,OLED_CS,OLED_RST,0);
      if (mHaveScreen) {
         mTSensor = new tapSensor(TAP_PIN,TAP_SUMTIME,TAP_NUMDATA);
         if (mTSensor) {
            mTSensor->begin();
            ourSleepMgr.setupSleep(AWAKE_MS,DIMMING_MS);
         }
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
      idleLight.setOnOff(true);                             // Start up our running light. Its all we got.
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

   rect  bombRect;
   
   crash = true;
   if (mHaveScreen) {
      bombRect.setRect(0,0,96,64);
      bmpPipe bomb(&bombRect);
      bomb.openPipe("/bomb9664.bmp");
      bomb.drawImage(0,0);
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

  bool      refresh;
  colorObj  aColor;
   
   if (mTSensor) {                                          // If we have a tap sensor..
      if (mTSensor->getTapVal()>=TAP_VAL) {                 // If we sense a tap..
         ourSleepMgr.wakeup();                              // We need to wake up.
      }
   }
   if (mHaveScreen && ding()) {                             // If we have a screen and its time to check stuff..
      if (ourSleepMgr.checkSleep()!=percBlack) {            // If we need to change the dimming value of the display..
         percBlack = ourSleepMgr.checkSleep();              // Set the new diming value.
         mMoisture->setNeedRefresh();                       // This is SUCH the sad hack. We need to redraw everthing. 
         mLimit->setNeedRefresh();                          // This is first time coding a screen saver and there is
         mSlash->setNeedRefresh();                          // no "draw all" method. So we hadrcode for now.
         mKey->setNeedRefresh();                            //
         mLoggingInd->setNeedRefresh();                     //
         mState->setNeedRefresh();                          //
      }
      if (weAre!=sitting) {                              // If we are not sitting.. (IE Watering or Soaking)
         mState->setNeedRefresh();                       // We need to draw the state view, showing the progress bar.
      }
      refresh = false;                                   // Default to no refresh on these next guys.
      if (ourParamObj.getDryLimit()!=mLastLimit) {       // If the user changed the dry limit while we weren't looking..
         mLimit->setPercent(ourParamObj.getDryLimit());  // Set the display limit to match the new value.
         mLastLimit = ourParamObj.getDryLimit();         // Save the new value for later checking.
         refresh = true;                                 // And we're going to do some refreshing of the screen.
      }
      if (moisture!=mLastMoist||refresh) {               // If the moisture has changed, or we need to refresh anyway..
         setColorMap(ourParamObj.getDryLimit());         // Reset the color map to reflect the new dry limit.
         aColor = mColorMap->Map(moisture);              // Map our a color for the new moisture value.
         mMoisture->setColors(&aColor,&black);           // Set up the text colors for the drawing.
         mMoisture->setPercent(moisture);                // Draw the new percentage value.
         mLastMoist = moisture;                          // Save off the new percentage value for chekcing later on.
      }
      if (weAre!=mLastState) {                              // If we have changed state..
         mState->setState(weAre);                           // Change the state text.
         mLastState = weAre;                                // Save off the new state for checking later on.
      }
      if (isLogging()==mLoggingInd->holding) {              // If they match.. (Sadly don't know what I'm doing here.)
         mLoggingInd->logging(isLogging());                 // fix it.
      }                                                     // Otherwise, leave it alone!
      start();
   }
};
