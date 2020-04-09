#include <adafruit_1947_Obj.h>
#include <Adafruit_GFX.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <screen.h>

#define ON_TIME 200
#define LITE_DIA  30
#define BTN_SIZE  30
#define BEEP_PIN  23

// ***************************************
//                redGreenLite
// ***************************************


class redGreenLite :  public drawObj,
                      public idler,
                      public  timeObj {

  public:
          redGreenLite(rect* inRect);
          ~redGreenLite(void);

          void  setLight(bool redGreen);
  virtual void  drawSelf(void);
  virtual void  idle(void);

          bool  liteOn;
          bool  liteColor;
};



// ***************************************


redGreenLite::redGreenLite(rect* inRect) 
  : drawObj(inRect),
  timeObj(ON_TIME) {

  hookup();
  liteOn = false;
}

  
redGreenLite::~redGreenLite(void) {  }


void redGreenLite::setLight(bool redGreen) {

  liteColor = redGreen;
  liteOn = true;
  start();
  setNeedRefresh();
}


void redGreenLite::drawSelf(void) {

  colorObj  aColor;
  
  if (liteOn) {
    if (liteColor) {
      aColor.setColor(LC_RED);
    } else {
      aColor.setColor(LC_GREEN);
    }
  } else {
    aColor.setColor(LC_BLACK);
  }
  screen->fillCircle(x+(width/2),y+(height/2),width,&aColor);
  aColor.setColor(LC_YELLOW);
  screen->drawCircle(x+(width/2),y+(height/2),width,&aColor);
  screen->drawCircle(x+(width/2+1),y+(height/2+1),width-2,&aColor);
}


void redGreenLite::idle(void) {

  if (liteOn) {
    if (ding()) {
      liteOn = false;
      setNeedRefresh();
    }
  }
}



// ***************************************
//                redGreenBtn
// ***************************************

class redGreenBtn : public drawObj {

  public:
          redGreenBtn(rect* inRect);
          ~redGreenBtn(void);

          void  setStuff(redGreenLite* inLight,redGreenBtn* aButton,redGreenBtn* bButton);
          void  jiggle(bool inColor);
  virtual void  doAction(void);
  virtual void  drawSelf(void);

          bool          mOurColor;
          redGreenLite* mLight;
          redGreenBtn*  other1;
          redGreenBtn*  other2;
};


// ***************************************


redGreenBtn::redGreenBtn(rect* inRect)
  : drawObj(inRect,touchLift) {

  mLight = NULL;
  other1 = NULL;
  other2 = NULL;
}


redGreenBtn::~redGreenBtn(void) {  }


void redGreenBtn::setStuff(redGreenLite* inLight,redGreenBtn* aButton,redGreenBtn* bButton) {

  mLight = inLight;
  other1 = aButton;
  other2 = bButton;
  mOurColor = random(2);
}

  
void redGreenBtn::jiggle(bool inColor) { 
  
  if (random(4)) {
    mOurColor = !inColor;
  } else {
    mOurColor = inColor;
  }
}


void redGreenBtn::doAction(void) {

  tone(BEEP_PIN, 500,35);
  if (mLight) mLight->setLight(mOurColor);
  if (other1) other1->jiggle(mOurColor);
  if (other2) other2->jiggle(mOurColor);
}


void redGreenBtn::drawSelf(void) {

  colorObj  aColor;
  colorObj  sColor(LC_CHARCOAL);
  int       drawWidth;
  int       drawHeight;
  
  drawWidth = width - 2;
  drawHeight = height - 2;
  if (!clicked)
    aColor.setColor(LC_LIGHT_BLUE);
  else
    aColor.setColor(LC_DARK_GREY);
  screen->fillRoundRect(x+2,y+2,drawWidth,drawHeight,3,&sColor);
  screen->fillRoundRect(x,y,drawWidth,drawHeight,3,&aColor);
}

          

void setup() {

  rect  theRect;

  if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
    Serial.println("NO SCREEN!");
    Serial.flush();
    while(true);
  }
  randomSeed(analogRead(0));
  
  theRect.setRect(120-LITE_DIA,75,LITE_DIA,LITE_DIA);
  redGreenLite* theLight = new redGreenLite(&theRect);

  theRect.setRect(120-BTN_SIZE/2-75,150,BTN_SIZE,BTN_SIZE);
  redGreenBtn* button1 = new redGreenBtn(&theRect);
  
  theRect.setRect(120-BTN_SIZE/2,150,BTN_SIZE,BTN_SIZE);
  redGreenBtn* button2 = new redGreenBtn(&theRect);
  
  theRect.setRect(120-BTN_SIZE/2+75,150,BTN_SIZE,BTN_SIZE);
  redGreenBtn* button3 = new redGreenBtn(&theRect);

  button1->setStuff(theLight,button2,button3);
  button2->setStuff(theLight,button1,button3);
  button3->setStuff(theLight,button1,button2);

  viewList.addObj(theLight);
  viewList.addObj(button1);
  viewList.addObj(button2);
  viewList.addObj(button3);
  
  screen->fillScreen(&white);
  ourEventMgr.begin();
}

void loop() {
  idle();

}
