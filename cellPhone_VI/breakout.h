#ifndef breakout_h
#define breakout_h

#include <label.h>
#include <panel.h>
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
#define MESSAGE_Y 120
#define MESSAGE_W 78
#define MESSAGE_H 8

#define BALLS_TXT_X TFT_W - 80 //40
#define BALLS_TXT_W 72
#define BALLS_Y     TFT_H - 10 //118
#define BALLS_H     8
#define BALLS_NUM_X BALLS_TXT_X + BALLS_TXT_W + 2
#define BALLS_NUM_W 16

#define NUM_BALLS 4

class breakout :  public panel {

  public:

  enum    userStates { nothin, pressinTDamnButton };
  enum    gameStates { preGame, inPLay, lostBall, gameOver, gameWin };
  
          breakout(void);
  virtual ~breakout(void);



  virtual void setup();
          void fillBricks(void);
          void doPaddle(void);
          void doBall(void);
          void showBallNum(int balls);
          void setState(gameStates state);
  virtual void loop(void);

          
          movingObj*      paddle;
          ballObj*        theBall;
          label*          message;
          label*          numBallsTxt;
          label*          numBallsNum;
          colorMultiMap   ballNumCMap;
          timeObj*        frameTimer;
          timeObj*        paddleTimer;
          timeObj*        textTimer;
          colorObj        backColor;
          gameStates      gameState;
          userStates      userState; 
          int             oldLoc;
          int             savedPaddleX;
          int             ballCount;
          bool            buttonHit;
};

#endif
