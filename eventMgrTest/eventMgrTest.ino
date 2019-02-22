  
#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_SSD1351.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <drawObj.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <lineObj.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <baseGraphics.h>
#include <displayObj.h>
#include <bmpPipe.h>
#include <screen.h>
#include <eventMgr.h>

#include <colorRect.h>

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4


class clickRect :  public colorRect {

  public:
            clickRect(int inLocX,int inLocY,word inWidth,word inHeight,int inset=0);
    virtual ~clickRect(void);
    virtual void  doAction();
    
};

  
clickRect::clickRect(int inLocX,int inLocY,word inWidth,word inHeight,int inset)
  : colorRect(inLocX,inLocY, inWidth,inHeight) {

  setEventSet(fullClick);  
}


clickRect::~clickRect(void) { }

void clickRect::doAction() {

    Serial.println("POW!");Serial.flush();
}




// ***************************************


class dragRect :  public drawGroup {

  public:
            dragRect(int inLocX,int inLocY,word inWidth,word inHeight,int inset=0);
    virtual ~dragRect(void);
    virtual void  eraseSelf(void);
    virtual void    doAction(event* inEvent,point* locaPt);
    virtual void    drawSelf(void);
            point startPoint;
};

  
dragRect::dragRect(int inLocX,int inLocY,word inWidth,word inHeight,int inset)
  : drawGroup(inLocX,inLocY, inWidth,inHeight) {

  setEventSet(dragEvents);  
}

void dragRect::eraseSelf(void) {

    point curPos;

    curPos.x = x;
    curPos.y = y;
    x = lastX;
    y = lastY;
    screen->fillRect(this,&black);
    x = curPos.x;
    y = curPos.y;
}


void dragRect::drawSelf(void) { screen->fillRect(this,&red); }


void dragRect::doAction(event* inEvent,point* locaPt) {

  if (inEvent->mType==dragBegin) {
    startPoint.x = x;
    startPoint.y = y;
  } else if (inEvent->mType==dragOn) {
    int newX = startPoint.x + inEvent->mXDist;
    int newY = startPoint.y + inEvent->mYDist;
    setLocation(newX,newY);
  }
}


dragRect::~dragRect(void) { }


clickRect* cRect; 
dragRect* dRect;
colorRect*  aRect;

void setup() {
  
   
   // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }

  if (!SD.begin(SD_CS)) {     // Bring the diskdrive online.
    while(true);
  }

  ourEventMgr.begin();                      // Kickstart our event manager.
  
  Serial.println("Everything's running."); 
  screen->fillScreen(&black);
  
  dRect = new dragRect(75,200, 50,50);
  
  aRect = new colorRect(5,5,30,30,1);
  aRect->setColor(LC_YELLOW);
  aRect->setEventSet(dragEvents);
  dRect->addObj(aRect);
  
  viewList.addObj(dRect);
  cRect = new clickRect(50,50, 50,50);
  cRect->setColor(LC_GREEN);
  
  viewList.addObj(cRect);

   
}

void loop() {
  idle();

}
