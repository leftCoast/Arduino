#include <drawObj.h>

#include "breakout.h"
#include "paddleObj.h"
#include "ballObj.h"
#include "brickObj.h"

#include  "../../cellOS.h"

// ******************************

breakout::breakout(void)
  : cellOSPanel(breakoutApp) {

  frameTimer    = new timeObj(FRAME_MS);
  paddleTimer   = new timeObj(PADDLE_MS);
  textTimer     = new timeObj(TEXT_MS);
  oldLoc        = -10;
  brickIndex    = 0;        // It was originally written to run once.
}


breakout::~breakout(void) {	// I think the brick list is leaking memory.

  if(frameTimer) { delete frameTimer; }
  if(paddleTimer) { delete paddleTimer; }
  if(textTimer) { delete textTimer; }
}


void breakout::setup() {

  backColor.setColor(&black);
  screen->fillScreen(&backColor);
  screen->drawRect(0,GAME_TOP,TFT_W,GAME_H,&white);   // Damn, looks good leave it!

  paddle = new paddleObj(54,PADDLE_Y,PADDLE_WIDTH,PADDLE_HEIGHT);
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
  y = BOTTOM_BRICK;
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(GREEN_BAR);
      aBrick->setBackColor(&backColor);
      addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(PURPLE_BAR);
      aBrick->setBackColor(&backColor);
      addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(RED_BAR);
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
      newLoc = touchPt.x - PADDLE_HWIDTH;
      if (newLoc<=0) {
        newLoc = 1; 
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
      while(screen->touched()) delay(10);
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
        if (screen->touched()) {
          sleep(500);
          setState(inPLay);
          doBall();
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
}
