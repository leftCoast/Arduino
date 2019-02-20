  
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


class dragRect :  public colorRect {

  public:
            dragRect(int inLocX,int inLocY,word inWidth,word inHeight,int inset=0);
    virtual ~dragRect(void);
    virtual void  doAction();
    
};

  
dragRect::dragRect(int inLocX,int inLocY,word inWidth,word inHeight,int inset)
  : colorRect(inLocX,inLocY, inWidth,inHeight) {

  setEventSet(fullClick);  
}


dragRect::~dragRect(void) { }

void dragRect::doAction() {

    Serial.println("POW!");Serial.flush();
}


dragRect* aRect; 


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

  aRect = new dragRect(50,50, 100,100);
  aRect->setColor(LC_GREEN);
  
  viewList.addObj(aRect);
}

void loop() {
  idle();

}
