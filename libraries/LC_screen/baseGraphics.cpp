#include <baseGraphics.h>

#define DEF_LOC_X  16    // Just anything so the user can see something.
#define DEF_LOC_Y  16
#define DEF_SIZE_X 16
#define DEF_SIZE_Y 16


float distance(point ptA,point ptB) {

	float widthSq;
	float heighSq;
	
	widthSq = (ptA.x - ptB.x)^2;
	heighSq = (ptA.y - ptB.y)^2;
	return sqrt(widthSq + heighSq);
}


/*
// HANDY for tracing issues through the drawing code.
void rect::printRect(char* label) {

	Serial.print(label);
	Serial.print("x:");Serial.print(x);Serial.print(" ");
	Serial.print("y:");Serial.print(y);Serial.print(" ");
	Serial.print("w:");Serial.print(width);Serial.print(" ");
	Serial.print("h:");Serial.println(height);
}
*/

rect::rect(void) {
   
   setLocation(DEF_LOC_X,DEF_LOC_Y);
   setSize(DEF_SIZE_X,DEF_SIZE_Y);
 }
 
 
rect::rect(rect* inRect) { setRect(inRect); }


rect::rect(int inX, int inY, word inWidth,word inHeight) {
   
   setRect(inX,inY,inWidth,inHeight);
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


void rect::setRect(int inX, int inY, int inWidth,int inHeight) {

	setLocation(inX,inY);
   setSize(inWidth,inHeight);
}


void rect::insetRect(int inset) {

	x = x + inset;
	y = y + inset;
	width = width - (2*inset);
	height = height - (2*inset);
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


bool rect::inRect(point* inPoint) { return inRect(inPoint->x,inPoint->y); }


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

