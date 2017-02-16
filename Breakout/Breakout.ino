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

#include "ballObj.h"
#include "brickObj.h"

#define PADDLE_Y      115
#define PADDLE_WIDTH  16
#define PADDLE_HEIGHT 4

#define FRAME_MS  10
#define LOST_MS   1000


movingObj  paddle(54,PADDLE_Y,PADDLE_WIDTH,PADDLE_HEIGHT);
label      debug(20,120,80,8);
ballObj    theBall(&paddle,&debug);

label    score(35,70,70,8);

mapper   pMapper(0,1023,0,128-PADDLE_WIDTH);
timeObj  frameTimer(FRAME_MS);
timeObj  lostTimer(LOST_MS);

colorObj  backColor;  // Everyone.
int oldLoc = -1;      // For paddle I think.

void setup() {

  //Serial.begin(9600); //while(!Serial);
  if (!initScreen(ADAFRUIT_1431,INV_PORTRAIT)) {
    while(true); // Kill the process.
  }
  backColor.setColor(&black);
  screen->fillScreen(&backColor);
  paddle.setBackColor(&backColor);
  viewList.addObj(&paddle);
 
  score.setColors(&yellow,&backColor);
  viewList.addObj(&score);
  
  debug.setColors(&yellow,&backColor);
  viewList.addObj(&debug);

  theBall.setBackColor(&backColor);
  viewList.addObj(&theBall);
  fillBricks();
  theBall.reset();
  frameTimer.start(); 
}

void fillBricks(void) {

  brickObj* aBrick;

  int y = BOTTOM_BRICK;
  colorObj aColor;
  y = BOTTOM_BRICK;
  aColor.setColor(&blue);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+1,y);
      aBrick->setColor(&aColor);
      viewList.addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  aColor.setColor(&yellow);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+1,y);
      aBrick->setColor(&aColor);
      viewList.addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  aColor.setColor(&red);
  for(byte j=0;j<1;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+1,y);
      aBrick->setColor(&aColor);
      viewList.addObj(aBrick);
    }
    y = y-BRICK_H;
  }
}
  


void loop() {

    int val;
    int newLoc;
    
    idle();
    if (frameTimer.ding()) {
      frameTimer.stepTime();
      val = analogRead(15);
      newLoc = pMapper.Map(val);
      if (newLoc!=oldLoc) {
        paddle.setLocation(newLoc,PADDLE_Y);
        oldLoc = newLoc;
      }
      if (theBall.ballLost) {
        if (lostTimer.ding()) {
          score.setValue("           ");
          theBall.reset();
          frameTimer.start();
        }
      } else {
        theBall.ballFrame();
        if (theBall.ballLost) {
          score.setValue("BALL LOST!!");
          lostTimer.start();
        }
      }
    }
  }
