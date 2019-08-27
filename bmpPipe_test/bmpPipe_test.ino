#include <Adafruit_GFX.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <gfxfont.h>
#include <SD.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <bmpPipe.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#define TFT_CS  10
#define TFT_RST -1
#define SD_CS   4

class bmpButton : public drawObj,
                  public bmpPipe {

  public :
                bmpButton(int x, int y,int width, int height);           
  virtual       ~bmpButton(void);
  
          void  beep(bool beepUp);
  virtual bool  acceptEvent(event* inEvent,point* locaPt);
  virtual void  drawSelf(void);
};


bmpButton::bmpButton(int x, int y,int width, int height)
  : drawObj(x,y,width,height,touchLift),
  bmpPipe(rect(0,0,width,height)) { }

  
bmpButton::~bmpButton(void) {  }


#define BEEP_PIN    23
#define BEEP_LOOPS  4
#define BEEP_CONST  500
void bmpButton::beep(bool beepUp) {

  unsigned int dTime;
  if (beepUp) {
    for(int i=0;i<BEEP_LOOPS;i++) {
      dTime = BEEP_CONST*(BEEP_LOOPS-i);
      digitalWrite(BEEP_PIN, LOW);
      delayMicroseconds(dTime);
      digitalWrite(BEEP_PIN, HIGH);
      delayMicroseconds(dTime);
    }
  } else {
    for(int i=0;i<BEEP_LOOPS;i++) {
      dTime = BEEP_CONST*BEEP_LOOPS;
      digitalWrite(BEEP_PIN, LOW);
      delayMicroseconds(dTime);
      digitalWrite(BEEP_PIN, HIGH);
      delayMicroseconds(dTime);
    }
  }
}


void bmpButton::drawSelf(void) {

  rect  aRect(0,0,width,height);
  
  if (!clicked) {
      aRect.setLocation(width,0);
    }
    setSourceRect(&aRect);
    drawBitmap(x,y);
}

bool bmpButton::acceptEvent(event* inEvent,point* locaPt) {

  switch (mEventSet) {
    case noEvents   : return false;             // noEvents, pass on..
    case touchLift    :                         // Classic button events, clicked lets you draw clicked.
      if (inEvent->mType==touchEvent) {         // If its a touch..
        if (inRect(locaPt)) {                   // - and if its on us..
          clicked   = true;                     // Might want to show we're clicked on.
          beep(clicked);
          doAction(inEvent,locaPt);             // Do our stuff.
          theTouched  = this;                   // Tell the world WE are accepting this event set.
          needRefresh = true;                   // touchLift doesn't get a lift event. So it needs the setRefresh here.
          return true;                          // Tell the world the event has been accepted.
        }
      } else if (theTouched==this && inEvent->mType==liftEvent) {   // We only want lifts if we'd accepted the touch.
        clicked   = false;                      // And we're no longer clicked.
        beep(clicked);
        doAction(inEvent,locaPt);               // Do our other stuff.
        needRefresh = true;                     // And here.. (see above)
        return true;                            // Again, tell the world the event has been accepted.
      }
      break;
    case fullClick  :               // Things like edit fields. A click changes their state.
      if (inEvent->mType==clickEvent) {   // If its a click event, that matches.
        if (inRect(locaPt)) {         // and if its on us..
          clicked   = false;          // No longer clicked by the time you see this.
          doAction();               // Do the action.
          return true;              // We don't set touched because this is a one shot event.
        }
      }
      break;
    case dragEvents :               // Things that move by touch.
      if (inEvent->mType==dragBegin) {      // If, the dragging finger has started..
        if (inRect(locaPt)) {         // and if its on us..
          doAction(inEvent,locaPt);     // Do our stuff.
          theTouched  = this;         // Tell the world WE are accepting this event set.
          return true;
        }
      } else if (inEvent->mType==dragOn) {    // still moving,
        doAction(inEvent,locaPt);         // Stil dragging? Keep drawing.
        return true;                  // Event has been accepted.
      } else if (inEvent->mType==liftEvent) { // Done dragging.
        doAction(inEvent,locaPt);         // Do our stuff.
        return true;                  // Again, tell the world the event has been accepted.
      }
      break;
    }
    return false;
  }
  
bmpButton* checkBtn;


void setup() {
  
  pinMode(BEEP_PIN, OUTPUT); 
  
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }

  Serial.println("Painting screen.");Serial.flush();
  screen->fillScreen(&black);
  
  if (!SD.begin(SD_CS)) {     // Bring the diskdrive online.
    Serial.println("NO SD drive.");Serial.flush();
    while(true);
  }
  
  ourEventMgr.begin();
  
  Serial.println("Opennig pipe.");Serial.flush();
  checkBtn = new bmpButton(20,60,22,22);
  if (checkBtn->openPipe("/icons/gChkHL22.bmp")) {
    Serial.println("Pipe open?");Serial.flush();
    checkBtn->beep(true);
    checkBtn->beep(false);
    viewList.addObj(checkBtn);
  } else {
    Serial.println("Pipe failed to open.");
    Serial.flush();
  }
}


void loop() {
  idle();
  //Serial.println("Ya!");
}
