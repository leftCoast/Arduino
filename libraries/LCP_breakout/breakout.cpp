#include <resizeBuff.h>
#include <strTools.h>
#include <drawObj.h>
#include <offscreen.h>
#include "breakout.h"
#include "paddleObj.h"
#include "ballObj.h"
#include "brickObj.h"
#include "toneObj.h"
#include <lilOS.h>

// ********* beepSounds ********

int	tonePin = -1;


void soundHitWall(void) {
	
	toneObj aTone(tonePin);

	aTone.play(1000,50);
}

void soundHitPaddel(void) {
	
	toneObj aTone(tonePin);

	aTone.play(750,50);
}


void soundHitBrick(void) {

	toneObj aTone(tonePin);

	aTone.play(1750,20);
}



void soundBallLost(void) {
	
	toneObj aTone(tonePin);

	aTone.play(500,35);
	while(aTone.isPlaying()) idle();
	aTone.play(250,35);
	while(aTone.isPlaying()) idle();
	aTone.play(1250,75);
}


void soundWinner(void) {
	
	toneObj	aTone(tonePin);
	int		eNote;
	tune		aTune;
	
	eNote = 100;
	for (int i=1;i<=4;i++) {
		aTune.addNote(NOTE_D4,eNote);
		aTune.addNote(NOTE_G4,eNote);
		aTune.addNote(NOTE_B5,eNote);
	}
	for (int i=1;i<=4;i++) {
		aTune.addNote(NOTE_E4,eNote);
		aTune.addNote(NOTE_G4,eNote);
		aTune.addNote(NOTE_C5,eNote);
	}
	for (int i=1;i<=4;i++) {
		aTune.addNote(NOTE_FS4,eNote);
		aTune.addNote(NOTE_A5,eNote);
		aTune.addNote(NOTE_D5,eNote);
	}
	aTune.addNote(NOTE_B5,eNote*2);
	aTune.addNote(NOTE_G4,eNote);
	aTune.addNote(NOTE_D4,eNote);
	aTune.addNote(NOTE_E4,eNote*2);
	aTune.addNote(NOTE_D4,eNote*2);
	aTune.addNote(NOTE_E4,eNote*2);
	aTune.addNote(NOTE_G4,eNote*6);
	aTune.startTune(&aTone);
	while(aTune.playing()) idle();
}


void soundZwoop(void) {

	toneObj	aTone(tonePin);

	
	for (int i = NOTE_C4;i<NOTE_C5;i++) {
		aTone.play(i,2);
		while(aTone.isPlaying()) idle();
	}
	
	while(aTone.isPlaying()) idle();
}

	
void soundGameOver(void) {

	toneObj	aTone(tonePin);
	
	aTone.play(NOTE_G3,1000);
	while(aTone.isPlaying()) idle();
	aTone.play(NOTE_C2,750);
	while(aTone.isPlaying()) idle();
}


void soundStartBall(void) {

	toneObj	aTone(tonePin);
	
	aTone.play(NOTE_G6,30);
	while(aTone.isPlaying()) idle();
	aTone.play(NOTE_C5,30);
}


// ******************************

breakout::breakout(int ourAppID)
  : panel(ourAppID) {

	tonePin = OSPtr->getTonePin();
	setupBrickBitmaps();
	frameTimer    = new timeObj(FRAME_MS);
	paddleTimer   = new timeObj(PADDLE_MS);
	textTimer     = new timeObj(TEXT_MS);
	oldLoc        = -10;
	brickIndex    = 0;        // It was originally written to run once.
}


breakout::~breakout(void) {	// I think the brick list is leaking memory.

	if(greenBmp) { delete greenBmp; }
	if(purpBmp) { delete purpBmp; }
	if(redBmp) { delete redBmp; }
	if(frameTimer) { delete frameTimer; }
	if(paddleTimer) { delete paddleTimer; }
	if(textTimer) { delete textTimer; }
}


// We need three bitmaps for doing the bars to knock out.
void breakout::setupBrickBitmaps(void) {
	
	char*			folderPtr;
	char*			pathPtr;
	int			pathLen;
	offscreen	vPort; 
	bmpObj*		BMPobj;
	
	folderPtr = NULL;
	pathPtr	 = NULL;
	greenBmp = new bitmap(BRICK_W,BRICK_H);				// Allocate the bitmaps.
	purpBmp	= new bitmap(BRICK_W,BRICK_H);
	redBmp	= new bitmap(BRICK_W,BRICK_H);
	if (greenBmp && purpBmp && redBmp) {							// If we got all three..
		if (heapStr(&folderPtr,OSPtr->getPanelFolder(mPanelID))) {				// Ask the OS for our folder path.
			pathLen = strlen(folderPtr);									// Num chars in this path..
			pathLen = pathLen + 13;											// Add about 13 more for the file name.
			if (resizeBuff(pathLen,&pathPtr)) {							// If we can allocate room for the full pathname.
				strcpy(pathPtr,folderPtr);									// Start with the folder path.
				strcat(pathPtr,"grnBar.bmp");								// Add the file name.
				BMPobj = new bmpObj(0,0,BRICK_W,BRICK_H,pathPtr);	// Create the bmpObj.
				if (BMPobj) {													// If we can read this file..
					vPort.beginDraw(greenBmp);								// Set up to offscreen drawing to the bitmap.
					BMPobj->draw(); 											// Draw to it..
					vPort.endDraw();											// Restore normal drawing.
					delete(BMPobj);											// Recycle the .bmp file object.
				}																	//
				strcpy(pathPtr,folderPtr);									// Restart with the folder path.
				strcat(pathPtr,"purpBar.bmp");							// Add the file name.
				BMPobj = new bmpObj(0,0,BRICK_W,BRICK_H,pathPtr);	// Grab another..
				if (BMPobj) {													// If we can read this file..
					vPort.beginDraw(purpBmp);								// Set up to offscreen drawing to the bitmap.
					BMPobj->draw(); 											// Draw to it.
					vPort.endDraw();											// Restore normal drawing.
					delete(BMPobj);											// Recycle the .bmp file object.
				}																	//
				strcpy(pathPtr,folderPtr);									// Restart with the folder path. Again, sigh..
				strcat(pathPtr,"redBar.bmp");								// Add the file name.
				BMPobj = new bmpObj(0,0,BRICK_W,BRICK_H,pathPtr);	// Grab the last one.
				if (BMPobj) {													// If we can read this file..
					vPort.beginDraw(redBmp);								// Set up to offscreen drawing to the bitmap.
					BMPobj->draw(); 											// Draw to it..
					vPort.endDraw();											// Restore normal drawing.
					delete(BMPobj);											// Recycle the .bmp file object.
				}
				resizeBuff(0,&pathPtr);										// Recycle reuse.
			}																		// 
			freeStr(&folderPtr);												// Recycle local path buff.
		}
	}
}
		


void breakout::setup(void) {

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
      aBrick->setColor(greenBmp);
      aBrick->setBackColor(&backColor);
      addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(purpBmp);
      aBrick->setBackColor(&backColor);
      addObj(aBrick);
    }
    y = y-BRICK_H;
  }
  for(byte j=0;j<2;j++) {
    for(byte i=0;i<NUM_BRICKS;i++) {
      aBrick = new brickObj((i*BRICK_W)+offset,y);
      aBrick->setColor(redBmp);
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
    aColor = ballNumCMap.map(balls);
    numBallsNum->setColors(&aColor,&backColor);
    numBallsNum->setValue(ballCount);
  } else {
    numBallsNum->setValue(" ");
  }
}


void breakout::setState(gameStates state) {

  switch(state) {
    case preGame :
      soundZwoop();
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
      soundBallLost();
      break;
    case gameOver :
      message->setValue("GAME OVER!!");
      showBallNum(ballCount);
      soundGameOver();
      textTimer->start();
      break;
    case gameWin :
      theBall->eraseSelf();
      message->setValue("WINNER!!");
      soundWinner();
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
