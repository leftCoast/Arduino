#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <baseGraphics.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

//#include <adafruit_1431_Obj.h> // ADAFRUIT_1431

#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <Adafruit_ILI9341.h>

#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <screen.h>
#include "movingObj.h"

#include "ballObj.h"
#include "brickObj.h"

#define TFT_W         240
#define TFT_H         320

#define PADDLE_WIDTH  16
#define PADDLE_HEIGHT 3
#define PADDLE_Y      TFT_H-16 //112

#define PADDLE_MS 20
#define FRAME_MS  10
#define TEXT_MS   1500

#define MESSAGE_X 25
#define MESSAGE_Y 70
#define MESSAGE_W 78
#define MESSAGE_H 8

#define BALLS_TXT_X TFT_W - 80 //40
#define BALLS_TXT_W 72
#define BALLS_Y     TFT_H - 10 //118
#define BALLS_H     8
#define BALLS_NUM_X BALLS_TXT_X + BALLS_TXT_W + 2
#define BALLS_NUM_W 16

#define NUM_BALLS 4

//#define OLED_CS     10
//#define OLED_RST    6
//#define OLED_SDCS   -1    // Not wired

#define TFT_CS     10
#define TFT_RST    6

//#define BUTTON_PIN  4
//#define POT_ANLG    A0 

enum gameStates { preGame, inPLay, lostBall, gameOver, gameWin };

gameStates    gameState;
movingObj     paddle(54,PADDLE_Y,PADDLE_WIDTH,PADDLE_HEIGHT);
ballObj       theBall(&paddle);
label         message(MESSAGE_X,MESSAGE_Y,MESSAGE_W,MESSAGE_H);
label         numBallsTxt(BALLS_TXT_X,BALLS_Y,BALLS_TXT_W,BALLS_H);
label         numBallsNum(BALLS_NUM_X,BALLS_Y,BALLS_NUM_W,BALLS_H);
colorMultiMap ballNumCMap;
//mapper        pMapper(1023,0,1,127-PADDLE_WIDTH);
timeObj       frameTimer(FRAME_MS);
timeObj       paddleTimer(PADDLE_MS);
timeObj       textTimer(TEXT_MS);
runningAvg    smoother(4);
colorObj      backColor;  
int           oldLoc = -1;
int           savedPaddleX;
int           ballCount;

enum        userStates { nothin, pressinTDamnButton };
userStates  userState = nothin;
timeObj     debounceTimer(100);
bool        buttonHit = false;

void setup() {

  //if (!initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
  //  while(true); // Kill the process.
  //}
  
  // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }
  
  backColor.setColor(&black);
  screen->fillScreen(&backColor);
  screen->drawRect(0,0,TFT_W,TFT_H,&white);   // Damn, looks good leave it!
  
  paddle.setBackColor(&backColor);
  viewList.addObj(&paddle);
 
  message.setColors(&yellow,&backColor);
  message.setJustify(TEXT_CENTER);
  viewList.addObj(&message);

  numBallsTxt.setColors(&white,&backColor);
  numBallsTxt.setJustify(TEXT_RIGHT);
  numBallsTxt.setValue("Balls:");
  numBallsTxt.drawSelf();           // In theroy we won't need redraws..
  //viewList.addObj(&numBallsTxt);  // So we can (not) link it up.

  numBallsNum.setColors(&backColor,&backColor); // Not yet..
  numBallsNum.setJustify(TEXT_LEFT);
  numBallsNum.setValue(" ");
  viewList.addObj(&numBallsNum);
  
  ballNumCMap.addColor(NUM_BALLS,&green);
  ballNumCMap.addColor(2,&yellow);
  ballNumCMap.addColor(1,&red);
    
  theBall.setBackColor(&backColor);
  viewList.addObj(&theBall);
  fillBricks();
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
  setState(preGame); 
}


void fillBricks(void) {

  brickObj* aBrick;
  int       y;
  int       offset;
  
  //y = BOTTOM_BRICK;
  offset = (TFT_W-(NUM_BRICKS*BRICK_W))/2;
  colorObj aColor;
  y = BOTTOM_BRICK;
  aColor.setColor(&blue);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(&aColor);
      aBrick->setBackColor(&backColor);
      viewList.addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  aColor.setColor(&yellow);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(&aColor);
      aBrick->setBackColor(&backColor);
      viewList.addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  aColor.setColor(&red);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(&aColor);
      aBrick->setBackColor(&backColor);
      viewList.addObj(aBrick);
    }
    y = y-BRICK_H;
  }
}


void doPaddle(void) {

    float val;
    int newLoc;
    point touchPt;
    
    if (paddleTimer.ding()) {
      paddleTimer.stepTime();
      //val = smoother.addData(analogRead(POT_ANLG));
      //newLoc = round(pMapper.Map(val));
      if(screen->touched()) {
        touchPt = screen->getPoint();
        newLoc = touchPt.x;
        if (newLoc!=oldLoc) {
          paddle.setLocation(newLoc,PADDLE_Y);
          oldLoc = newLoc;
        }
      }
    }
  }


void doBall(void) {

    if (frameTimer.ding()) {
      frameTimer.stepTime();
      theBall.ballFrame();
      if (theBall.ballLost) {
          ballCount--;
          if (ballCount) {
            setState(lostBall);
          } else {
            setState(gameOver);
          }
      } else {
        if (!bricks()) {
          setState(gameWin);
        }
      }
    }
  }


void showBallNum(int balls) {

  colorObj  aColor;

  if (balls) {
    aColor = ballNumCMap.Map(balls);
    numBallsNum.setColors(&aColor,&backColor);
    numBallsNum.setValue(ballCount);
  } else {
    numBallsNum.setValue(" ");
  }
}


void setState(gameStates state) {

    switch(state) {
      case preGame :
        buttonHit = false;
        paddleTimer.start();
        ballCount = NUM_BALLS;
        showBallNum(ballCount);
        theBall.setLocation(BALL_X,BALL_Y);
        resetBricks();
        message.setValue("Tap to start");
        while(!paddleTimer.ding());
        doPaddle();
        savedPaddleX = paddle.x;
        frameTimer.start();
        break;
      case inPLay :
        buttonHit = false;
        message.setValue(" ");
        theBall.reset();
        frameTimer.start();
        break;
      case lostBall :
        message.setValue("BALL LOST!!");
        showBallNum(ballCount);
        textTimer.start();
        break;
      case gameOver :
        message.setValue("GAME OVER!!");
        showBallNum(ballCount);
        textTimer.start();
        break;
      case gameWin :
        theBall.eraseSelf();
        message.setValue("WINNER!!");
        textTimer.start();
        break;
    }
    gameState = state;
  }


void loop(void) {

    idle();
    doPaddle();
    switch(gameState) {
      case preGame :
          if (buttonHit) {
            setState(inPLay);
          }
        break;
      case inPLay : doBall(); break;
      case lostBall :
        if (textTimer.ding()) {
          setState(inPLay);
        }
        break;
      case gameOver :
        if (textTimer.ding()) {
          setState(preGame);
        }
        break;
      case gameWin :
        if (textTimer.ding()) {
          setState(preGame);
        }
        break;
    }
    switch (userState) {
    case nothin : 
      //if (!digitalRead(BUTTON_PIN)) {
      if (screen->touched()) {
        userState = pressinTDamnButton;
        debounceTimer.start();
        buttonHit = true;
      }
      break;
    case pressinTDamnButton : 
      if (debounceTimer.ding()) {
        //if (digitalRead(BUTTON_PIN)) {  // Is the bozo holding the damn button down?
        if (screen->touched()) {
          userState = nothin;
        } else {
          debounceTimer.start();        // Whatever, I'll come back later..
        }
      }
      break;
    }
  }
