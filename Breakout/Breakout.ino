#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_ILI9341.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <adafruit_1431_Obj.h> // ADAFRUIT_1431
//#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <bmpPipe.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>
#include "movingObj.h"

class ballObj : public movingObj {

  public:
    movingObj(
  
};




#define PADDLE_Y      110
#define PADDLE_WIDTH  16
#define PADDLE_HEIGHT 4

movingObj  paddle(54,PADDLE_Y,PADDLE_WIDTH,PADDLE_HEIGHT);
//label    test(30,50,40,8);
mapper   pMapper(0,1023,0,128-PADDLE_WIDTH);
timeObj  frameTimer(50);

void setup() {

  Serial.begin(9600); while(!Serial);
  if (!initScreen(ADAFRUIT_1431,INV_PORTRAIT)) {
    while(true); // Kill the process.
  }
  screen->fillScreen(&black);
  paddle.setBackColor(&black);
  viewList.addObj(&paddle);
  //test.setColors(&black,&green);
  //viewList.addObj(&test);

  frameTimer.start();
}

int oldLoc = -1;

void loop() {

  int val;
  int newLoc;
  
  rect  aRect;
  idle();
  if (frameTimer.ding()) {
    frameTimer.stepTime();
    val = analogRead(15);
    //test.setValue(val);
    newLoc = pMapper.Map(val);
    if (newLoc!=oldLoc) {
      paddle.setLocation(newLoc,PADDLE_Y);
      oldLoc = newLoc;
    }
  }

}
