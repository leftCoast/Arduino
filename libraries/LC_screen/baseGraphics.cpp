#include <baseGraphics.h>

#define DEF_LOC_X  16    // Just anything so the user can see something.
#define DEF_LOC_Y  16
#define DEF_SIZE_X 16
#define DEF_SIZE_Y 16


rect::rect(void) {
   
   setLocation(DEF_LOC_X,DEF_LOC_Y);
   setSize(DEF_SIZE_X,DEF_SIZE_Y);
 }
 
 
rect::rect(int inX, int inY, word inWidth,word inHeight) {
   
   setAll(inX,inY,inWidth,inHeight);
}

 
rect::~rect(void) { }
 
 
void  rect::setLocation(int inX, int inY) {
  
  x = inX;
  y = inY;
}


void  rect::setSize(word inWidth,word inHeight) {

  width = inWidth;
  height = inHeight;
}


void rect::setRect(rect* inRect) {
  
  setLocation(inRect->x,inRect->y);
  setSize(inRect->width,inRect->height);
}


void rect::setRect(point* inPt1,point* inPt2) {

  int locX = min(inPt1->x,inPt2->x);
  int locY = max(inPt1->y,inPt2->y);
  word width = abs((int)inPt1->x-(int)inPt2->x);
  word height = abs((int)inPt1->y-(int)inPt2->y);
  setLocation(locX,locY);
  setSize(width,height);
}


void rect::setAll(int inX, int inY, word inWidth,word inHeight) {

	setLocation(inX,inY);
   setSize(inWidth,inHeight);
}


int rect::maxX(void) { return(x + width); }
int rect::maxY(void)  { return(y + height); }
int rect::minX(void) { return(x); }
int rect::minY(void)  { return(y); }

bool rect::inRect(int inX, int inY) {

  return(
  inX >= minX() &&
  inX <= maxX() &&
  inY >= minY() &&
  inY <= maxY()
  );
}


point rect::getCorner(rectPt corner) {

	point	tempPt;
	
	switch(corner) {
		case topLeftPt :
			tempPt.x = x;
			tempPt.y = y;
		break;
		case topRightPt :
			tempPt.x = x + width;
			tempPt.y = y;
		break;
		case bottomLeftPt :
			tempPt.x = x;
			tempPt.y = y + height;
		break;
		case bottomRightPt :
			tempPt.x = x + width;
			tempPt.y = y + height;
		break;
	}
	return tempPt;
}


// Are we touching this passed in rectangle?
bool rect::overlap(rect* checkRect) {

		if(maxX()<checkRect->minX()) return false;
		if(minX()>checkRect->maxX()) return false;
		if(maxY()<checkRect->minY()) return false;
		if(minY()>checkRect->maxY()) return false;
		return true;
	}


