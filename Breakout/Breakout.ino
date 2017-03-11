//#include <Adafruit_GFX.h>
//#include <gfxfont.h>

//#include <Adafruit_ILI9341.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

//#include <adafruit_1431_Obj.h> // ADAFRUIT_1431
//#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SSD_13XX_Obj.h>
#include <SSD_13XX.h>

#include <bmpPipe.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>
#include "movingObj.h"

#include "ballObj.h"
#include "brickObj.h"

#define PADDLE_WIDTH  16
#define PADDLE_HEIGHT 3
#define PADDLE_Y      112

#define PADDLE_MS 20
#define FRAME_MS  10
#define TEXT_MS   1500

#define MESSAGE_X 25
#define MESSAGE_Y 70
#define MESSAGE_W 78
#define MESSAGE_H 8

#define BALLS_TXT_X 40
#define BALLS_TXT_W 72
#define BALLS_Y     118
#define BALLS_H     8
#define BALLS_NUM_X BALLS_TXT_X + BALLS_TXT_W + 2
#define BALLS_NUM_W 16

#define NUM_BALLS 4

#define BUTTON_PIN  21

enum gameStates { preGame, inPLay, lostBall, gameOver, gameWin };

gameStates    gameState;
movingObj     paddle(54,PADDLE_Y,PADDLE_WIDTH,PADDLE_HEIGHT);
ballObj       theBall(&paddle);
label         message(MESSAGE_X,MESSAGE_Y,MESSAGE_W,MESSAGE_H);
label         numBallsTxt(BALLS_TXT_X,BALLS_Y,BALLS_TXT_W,BALLS_H);
label         numBallsNum(BALLS_NUM_X,BALLS_Y,BALLS_NUM_W,BALLS_H);
colorMultiMap ballNumCMap;
mapper        pMapper(0,1023,1,127-PADDLE_WIDTH);
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
bool          buttonHit = false;

//#include <soundCard.h>

//soundCard theSoundCard(soundCard_BREAKOUT);

void setup() {

  Serial.begin(9600); while(!Serial);
  Serial.println("Try screen.");
  if (!initScreen(SUMO_TOY_SSD_13XX,INV_PORTRAIT)) {
    Serial.print("Screen fail.");
    while(true); // Kill the process.
  }
  Serial.println("Screen worked!");

  SD.begin(20);
  backColor.setColor(&black);
  screen->fillScreen(&backColor);
  delay(3000);
  screen->drawRect(0,0,128,128,&white);   // Damn, looks good leave it!
  
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
  /*
  if (theSoundCard.begin()) {
    Serial.println("theSoundCard.begin(); success.");
    //if (theSoundCard.setSoundfile("Corvette.mp3")) { Serial.printf("set corvette");}
    //theSoundCard.command(play);
  } else {
    Serial.println("theSoundCard.begin(); FAIL.");
  }
  */
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setState(preGame); 
}


void fillBricks(void) {

  brickObj* aBrick;
  int y;;
  int offset;
  
  y = BOTTOM_BRICK;
  offset = (128-(NUM_BRICKS*BRICK_W))/2;
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
    
    if (paddleTimer.ding()) {
      paddleTimer.stepTime();
      val = smoother.addData(analogRead(15));
      newLoc = round(pMapper.Map(val));
      if (newLoc!=oldLoc) {
        paddle.setLocation(newLoc,PADDLE_Y);
        oldLoc = newLoc;
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
        message.setValue("Press start");
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
      if (!digitalRead(BUTTON_PIN)) {
        userState = pressinTDamnButton;
        debounceTimer.start();
        buttonHit = true;
      }
      break;
    case pressinTDamnButton : 
      if (debounceTimer.ding()) {
        if (digitalRead(BUTTON_PIN)) {  // Is the bozo holding the damn button down?
          userState = nothin;
        } else {
          debounceTimer.start();        // Whatever, I'll come back later..
        }
      }
      break;
    }
  }

