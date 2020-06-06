#include <baseGraphics.h>
#include <math.h>

#define DEF_LOC_X  16    // Just anything so the user can see something.
#define DEF_LOC_Y  16
#define DEF_SIZE_X 16
#define DEF_SIZE_Y 16

/*
void printPoint(point* inPt,char* name) {

	Serial.print(name);
	Serial.print(inPt->x);
	Serial.print(F(", "));
	Serial.println(inPt->y);
}
*/
	
int	xDistance(point ptA,point ptB) { return ptB.x - ptA.x; }


int	yDistance(point ptA,point ptB) { return ptB.y - ptA.y; }


float distance(point ptA,point ptB) {

	float widthSq;
	float heighSq;
	
	widthSq = pow(xDistance(ptA,ptB),2);
	heighSq = pow(yDistance(ptA,ptB),2);
	return sqrt(widthSq + heighSq);
}


float	angle(point ptA,point ptB) {

	float xDist;
	float yDist;
	float	angle;
		
	xDist = xDistance(ptA,ptB);
	yDist	= yDistance(ptA,ptB);
	if (xDist) {
		angle = atan(yDist/xDist);
	} else {
		angle = M_PI/2;
	}
	return angle;
}


/*
// HANDY for tracing issues through the drawing code.
void rect::printRect(char* label) {

	Serial.print(label);
	Serial.print(F("x:"));Serial.print(x);Serial.print(F(" "));
	Serial.print(F("y:"));Serial.print(y);Serial.print(F(" "));
	Serial.print(F("w:"));Serial.print(width);Serial.print(F(" "));
	Serial.print(F("h:"));Serial.println(height);
}
*/


rect::rect(void) {
   
   setLocation(DEF_LOC_X,DEF_LOC_Y);
   setSize(DEF_SIZE_X,DEF_SIZE_Y);
 }
 
 
rect::rect(rect* inRect) { setRect(inRect); }


rect::rect(int inX, int inY, int inWidth,int inHeight) {
   
   setRect(inX,inY,inWidth,inHeight);
}

 
rect::~rect(void) { }
 
 
void  rect::setLocation(int inX, int inY) {
  
  x = inX;
  y = inY;
}


void  rect::setSize(int inWidth,int inHeight) {

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
  int width = abs((int)inPt1->x-(int)inPt2->x);
  int height = abs((int)inPt1->y-(int)inPt2->y);
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


