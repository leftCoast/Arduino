#ifndef breakout_h
#define breakout_h

#include "toneObj.h"
#include <label.h>
#include <lilOS.h>

#include "paddleObj.h"
#include "ballObj.h"
#include "brickObj.h"

#define TFT_W         240
#define TFT_H         320

#define GAME_TOP      24
#define GAME_H        TFT_H-GAME_TOP

#define PADDLE_WIDTH  20
#define PADDLE_HWIDTH 10

#define PADDLE_HEIGHT 4
#define PADDLE_Y      GAME_H-40

#define PADDLE_MS 20
#define FRAME_MS  10
#define TEXT_MS   1500

#define MESSAGE_X 1
#define MESSAGE_Y GAME_TOP + 170
#define MESSAGE_W TFT_W - 2
#define MESSAGE_H 8

#define BALLS_TXT_X TFT_W - 100 //40
#define BALLS_TXT_W 72
#define BALLS_Y     GAME_H - 10
#define BALLS_H     8
#define BALLS_NUM_X BALLS_TXT_X + BALLS_TXT_W + 2
#define BALLS_NUM_W 16

// --------------------
#define NUM_BALLS 4

#define BALL_X    120
#define BALL_Y    GAME_TOP + 130
#define BALL_DIA  6
#define BALL_FPM  2   // Frames / move
#define BALL_DY   -2
#define BALL_DX   0

// --------------------

#define BRICK_W       34
#define BRICK_H       12
#define NUM_BRICKS    238/BRICK_W
#define BOTTOM_BRICK  GAME_TOP + 96
#define BRICK_ROWS    6
#define TOTAL_BRICKS  NUM_BRICKS*BRICK_ROWS


extern void soundHitWall(void);
extern void soundBallLost(void);
extern void soundHitPaddel(void);
extern void soundHitBrick(void);
extern void soundZwoop(void);
extern void soundGameOver(void);
extern void soundStartBall(void);


class ballObj;

class breakout :  public panel {

	public:

	enum		gameStates { preGame, inPLay, lostBall, gameOver, gameWin };

				breakout(int ourAppID);
	virtual	~breakout(void);

	virtual	void setup(void);
				void setupBrickBitmaps(void);
				void fillBricks(void);
				void doPaddle(void);
				void doBall(void);
				void showBallNum(int balls);
				void setState(gameStates state);
	virtual	void loop(void);

				bitmap*			greenBmp;
				bitmap*			purpBmp;
				bitmap*			redBmp;
				paddleObj*		paddle;
				ballObj*			theBall;
				label*			message;
				label*			numBallsTxt;
				label*			numBallsNum;
				colorMultiMap	ballNumCMap;
				timeObj*			frameTimer;
				timeObj*			paddleTimer;
				timeObj*			textTimer;
				colorObj			backColor;
				gameStates		gameState;
				int				oldLoc;
				int				savedPaddleX;
				int				ballCount;
};

#endif
