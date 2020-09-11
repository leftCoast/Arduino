
#include "ballObj.h"
#include "brickObj.h"
#include "breakout.h"

gameCompass ourCompass;


gameCompass::gameCompass(void) {

  courses[0].y = -2 ; courses[0].x = 0;
  courses[1].y = -2 ; courses[1].x = 1;
  courses[2].y = -3 ; courses[2].x = 2;
  courses[3].y = -2 ; courses[3].x = 2;
  courses[4].y = -2 ; courses[4].x = 3;

  courses[9].y = 2 ; courses[9].x = 0;
  courses[8].y = 2 ; courses[8].x = 1;
  courses[7].y = 3 ; courses[7].x = 2;
  courses[6].y = 2 ; courses[6].x = 2;
  courses[5].y = 2 ; courses[5].x = 3;

  courses[10].y = 2 ; courses[10].x = -1;
  courses[11].y = 3 ; courses[11].x = -2;
  courses[12].y = 2 ; courses[12].x = -2;
  courses[13].y = 2 ; courses[13].x = -3;

  courses[17].y = -2 ; courses[17].x = -1;
  courses[16].y = -3 ; courses[16].x = -2;
  courses[15].y = -2 ; courses[15].x = -2;
  courses[14].y = -2 ; courses[14].x = -3;
}


gameCompass::~gameCompass(void) {  }


point gameCompass::adjustCourse(int dy, int dx, courseChange change) {

  point aPoint;
  bool  foundPoint;
  int   index;

  aPoint.y = dy;    // Defualt, no change.
  aPoint.x = dx;
  index = 0;
  foundPoint = false;
  while (!foundPoint && index < COMPASS_POINTS) {
    foundPoint = (courses[index].y == dy && courses[index].x == dx);
    if (!foundPoint) {
      index++;
    }
  }
  if (foundPoint) {
    switch (change) {
      case clockwise :
        if (index != 4 && index != 13) {
          index++;
          if (index >= COMPASS_POINTS) {
            index = 0;
          }
          aPoint.y = courses[index].y;
          aPoint.x = courses[index].x;
        }
        break;
      case counterClockwise :
        if (index != 5 && index != 14) {
          index--;
          if (index < 0) {
            index = COMPASS_POINTS - 1;
          }
          aPoint.y = courses[index].y;
          aPoint.x = courses[index].x;
        }
        break;
    }
  }
  return aPoint;
}




// ************



ballObj::ballObj(paddleObj* inPaddle)
  : drawObj(BALL_X, BALL_Y, BALL_DIA, BALL_DIA) {

	thePaddle = inPaddle;
	ballLost = true;
}


ballObj::~ballObj(void) {  }


int ballObj::middleX(void) { return x+width/2; }


void ballObj::reset(void) {

  deltaY  = abs(BALL_DY);    // Always start heading down.
  deltaX = BALL_DX;
  framePerPix = BALL_FPM;

  yCount = 0;
  xCount = 0;
  frameCount = 0;
  drawObj::setLocation(BALL_X, BALL_Y);
  ballLost = false;
  soundStartBall();
}


void ballObj::setLocation(int inX, int inY) {

  boolean   clearSkies;
  boolean   done;
  point     newCourse;
  rect      ourNewSelf;
  int       index;
  boolean   theTouch;
  
  if (!ballLost) {
    clearSkies = true;

    // Fist we check the walls..
    if (inX >= TFT_W - width) { // Hit right wall
    	soundHitWall();
      deltaX = -deltaX;
      clearSkies = false;
    }
    if (inX <= width) { // Hit left wall
    	soundHitWall();
      deltaX = -deltaX;
      clearSkies = false;
    }
    if (inY >= (GAME_H-24) - height) {        // Hit bottom wall
      ballLost = true;                			// The ball was LOST!!
      screen->fillRect(x,y,width,height,this);
      clearSkies = false;
    }
    if (inY <= GAME_TOP+8 - height) { // Hit top wall
    	soundHitWall();
      deltaY = -deltaY;
      clearSkies = false;
    }

    // Then we check the paddle.
    if (y + height <= thePaddle->y && inY + height >= thePaddle->y) { // We're at the paddle level!
      if (!(inX > thePaddle->maxX() || inX + width < thePaddle->minX())) { // Paddle hit!!
			soundHitPaddel();
        	deltaY = -abs(deltaY);                                      // So as to not get stuck. Force and upward movement.
        
        if (middleX() > thePaddle->middleX()) {
          newCourse = ourCompass.adjustCourse(deltaY, deltaX, clockwise);
          deltaY = newCourse.y;
          deltaX = newCourse.x;
        } else if (middleX() < thePaddle->middleX()) {
          newCourse = ourCompass.adjustCourse(deltaY, deltaX, counterClockwise);
          deltaY = newCourse.y;
          deltaX = newCourse.x;
        }
        clearSkies = false;
      }
    }

    // Now we check the bricks!
    ourNewSelf.setRect(this);
    ourNewSelf.setLocation(inX,inY);
    theTouch = false;
    index = 0;
    done = false;
    while(!done) {                      // find the first brick we hit.
      if (index>=TOTAL_BRICKS) {
        done = true;
      } else {
        theTouch = ourNewSelf.overlap((rect*)brickList[index]);
        if (theTouch) {
          brickList[index]->hit();
          done = true;
        }
        index++;
      }
    }
    if (theTouch) {
      deltaY = -deltaY;
      clearSkies = false;
    }
    if (clearSkies) {
      drawObj::setLocation(inX, inY);
    }
  }
}


void ballObj::eraseSelf(void){ 

	if (lastX>0 && lastY>0) {
		screen->fillRoundRect(lastX,lastY,width,height,width/3,this);
	}
}


void ballObj::drawSelf(void) {

	colorObj	aColor(LC_RED);
	screen->fillRoundRect(x,y,width,height,width/3,&aColor);
}


void ballObj::ballFrame(void) {

  int newX;
  int newY;

  if (!ballLost) {
    if (frameCount == 0) {
      if (xCount < abs(deltaX)) {
        newX = x + deltaX;
        xCount++;
      } else {
        newX = x;
      }
      if (yCount < abs(deltaY)) {
        newY = y + deltaY;
        yCount++;
      } else {
        newY = y;
      }
      if (xCount >= abs(deltaX) && yCount >= abs(deltaY)) {
        xCount = 0;
        yCount = 0;
      }
      setLocation(newX, newY);
    }
    frameCount++;
    if (frameCount == framePerPix) {
      frameCount = 0;
    }
  }
}
