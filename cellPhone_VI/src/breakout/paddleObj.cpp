#include "paddleObj.h"

paddleObj::paddleObj(int x, int y, int width,int height) 
  :drawObj(x,y,width,height) { }	// Was true. I think that's wrong.

paddleObj::~paddleObj(void) {  }

void paddleObj::eraseSelf(void) { screen->fillRect(lastX,lastY,width,height,this); }

void paddleObj::drawSelf(void) { screen->fillRect(x,y, width, height, &white); }

int paddleObj::middleX(void) { return x+width/2; }
