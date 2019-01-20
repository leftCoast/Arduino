
#include "breakout.h"
#include "movingObj.h"
#include "ballObj.h"
#include "brickObj.h"

#include "cellOS.h"

#define TFT_W         240
#define TFT_H         320

#define PADDLE_WIDTH  20
#define PADDLE_HEIGHT 4
#define PADDLE_Y      TFT_H-40

#define PADDLE_MS 20
#define FRAME_MS  10
#define TEXT_MS   1500

#define MESSAGE_X 1
#define MESSAGE_Y 190
#define MESSAGE_W TFT_W - 2
#define MESSAGE_H 8

#define BALLS_TXT_X TFT_W - 100 //40
#define BALLS_TXT_W 72
#define BALLS_Y     TFT_H - 10 //118
#define BALLS_H     8
#define BALLS_NUM_X BALLS_TXT_X + BALLS_TXT_W + 2
#define BALLS_NUM_W 16

#define NUM_BALLS 4


breakout::breakout(void)
  : panel(breakoutApp,true) {
  
  frameTimer    = new timeObj(FRAME_MS);
  paddleTimer   = new timeObj(PADDLE_MS);
  textTimer     = new timeObj(TEXT_MS);
  oldLoc        = -1;
  userState     = nothin;
  buttonHit     = false;
}


breakout::~breakout(void) {

  if(frameTimer) { delete frameTimer; }
  if(paddleTimer) { delete paddleTimer; }
  if(textTimer) { delete textTimer; }
}


void breakout::setup() {

  backColor.setColor(&black);
  screen->fillScreen(&backColor);
  screen->drawRect(0,0,TFT_W,TFT_H,&white);   // Damn, looks good leave it!

  paddle = new movingObj(54,PADDLE_Y,PADDLE_WIDTH,PADDLE_HEIGHT);
  paddle->setBackColor(&backColor);
  addObj(paddle);

  message = new label(MESSAGE_X,MESSAGE_Y,MESSAGE_W,MESSAGE_H);
  message->setColors(&yellow,&backColor);
  message->setJustify(TEXT_CENTER);
  addObj(message);

  numBallsTxt = new label(BALLS_TXT_X,BALLS_Y,BALLS_TXT_W,BALLS_H);
  numBallsTxt->setColors(&white,&backColor);
  numBallsTxt->setJustify(TEXT_RIGHT);
  numBallsTxt->setValue("Balls:");
  addObj(numBallsTxt);
    
  numBallsNum = new label(BALLS_NUM_X,BALLS_Y,BALLS_NUM_W,BALLS_H);
  numBallsNum->setColors(&backColor,&backColor); // Not yet..
  numBallsNum->setJustify(TEXT_LEFT);
  numBallsNum->setValue(" ");
  addObj(numBallsNum);
  
  ballNumCMap.addColor(NUM_BALLS,&green);
  ballNumCMap.addColor(2,&yellow);
  ballNumCMap.addColor(1,&red);

  theBall = new ballObj(paddle);
  theBall->setBackColor(&backColor);
  addObj(theBall);
  fillBricks();
  setState(preGame); 
}


void breakout::fillBricks(void) {

  brickObj* aBrick;
  int       y;
  int       offset;
  
  offset = (TFT_W-(NUM_BRICKS*BRICK_W))/2;
  colorObj aColor;
  y = BOTTOM_BRICK;
  aColor.setColor(&blue);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(&aColor);
      aBrick->setBackColor(&backColor);
      addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  aColor.setColor(&yellow);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(&aColor);
      aBrick->setBackColor(&backColor);
      addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  aColor.setColor(&red);
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(&aColor);
      aBrick->setBackColor(&backColor);
      addObj(aBrick);
    }
    y = y-BRICK_H;
  }
}


void breakout::doPaddle(void) {

    int newLoc;
    point touchPt;
    
    if (paddleTimer->ding()) {
      paddleTimer->stepTime();
      if(screen->touched()) {
        touchPt = screen->getPoint();
        newLoc = touchPt.x;
        if (newLoc<0) {
          newLoc = 0; 
        } else if (newLoc>=TFT_W-PADDLE_WIDTH) {
          newLoc = TFT_W-PADDLE_WIDTH-1;
        }
        if (newLoc!=oldLoc) {
          paddle->setLocation(newLoc,PADDLE_Y);
          oldLoc = newLoc;
        }
      }
    }
  }


void breakout::doBall(void) {

    if (frameTimer->ding()) {
      frameTimer->stepTime();
      theBall->ballFrame();
      if (theBall->ballLost) {
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


void breakout::showBallNum(int balls) {

  colorObj  aColor;

  if (balls) {
    aColor = ballNumCMap.Map(balls);
    numBallsNum->setColors(&aColor,&backColor);
    numBallsNum->setValue(ballCount);
  } else {
    numBallsNum->setValue(" ");
  }
}


void breakout::setState(gameStates state) {

    switch(state) {
      case preGame :
        buttonHit = false;
        paddleTimer->start();
        ballCount = NUM_BALLS;
        showBallNum(ballCount);
        theBall->setLocation(BALL_X,BALL_Y);
        resetBricks();
        message->setValue("Tap to start");
        while(!paddleTimer->ding());
        doPaddle();
        savedPaddleX = paddle->x;
        frameTimer->start();
        break;
      case inPLay :
        buttonHit = false;
        message->setValue(" ");
        theBall->reset();
        frameTimer->start();
        break;
      case lostBall :
        message->setValue("BALL LOST!!");
        showBallNum(ballCount);
        textTimer->start();
        break;
      case gameOver :
        message->setValue("GAME OVER!!");
        showBallNum(ballCount);
        textTimer->start();
        break;
      case gameWin :
        theBall->eraseSelf();
        message->setValue("WINNER!!");
        textTimer->start();
        break;
    }
    gameState = state;
  }


void breakout::loop(void) {

    doPaddle();
    switch(gameState) {
      case preGame :
          if (buttonHit) {
            setState(inPLay);
          }
        break;
      case inPLay : doBall(); break;
      case lostBall :
        if (textTimer->ding()) {
          setState(inPLay);
        }
        break;
      case gameOver :
        if (textTimer->ding()) {
          setState(preGame);
        }
        break;
      case gameWin :
        if (textTimer->ding()) {
          setState(preGame);
        }
        break;
    }
    switch (userState) {
    case nothin : 
      if (screen->touched()) {
        userState = pressinTDamnButton;
        buttonHit = true;
      }
      break;
    case pressinTDamnButton : 
      if (screen->touched()) {
        userState = nothin;
      }
      break;
    }
  }
